/**
 * masterpasswordapp.cc
 * This file is part of MasterPassword-Material
 *
 * Copyright (c) 2015 Kilian Schweppe
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "masterpasswordapp.h"

#define SHARED_MEM_KEY "masterpassword"
#define LOGOUT_TIMER_INTERVAL 250

#include "importer.h"
#include "exporter.h"
#include "sitemenu.h"

MasterPasswordApp::MasterPasswordApp(int &argc, char *argv[])
    : QApplication(argc, argv),
      tray_icon_(QIcon(":/images/tray_icon"))
{
  shared_mem_.setKey(SHARED_MEM_KEY);
  if (!shared_mem_.create(1))
    qFatal("could not create shared memory");
  resetSharedMemory();

  setApplicationVersion(APP_VERSION);
  settings_.setAppVersion(APP_VERSION);

  QQmlContext *ctx = engine_.rootContext();

  // register MPW type
  mpw::registerQml();

  // app constants
  ctx->setContextProperty("AppVersion", APP_VERSION);
  ctx->setContextProperty("BuildTime", BUILDTIME);
  ctx->setContextProperty("BuildDate", BUILDDATE);
  ctx->setContextProperty("QtVersion", QT_VERSION_STR);

  // QML propertie
  ctx->setContextProperty("Backend", this);
  ctx->setContextProperty("Settings", &settings_);
  ctx->setContextProperty("ResetClipboardThread", &clipboard_thread_);

  // install translator
  if (translator_.load(QLocale::system().name(), ":/lang"))
  {
    qDebug("loading translation...");
    installTranslator(&translator_);
  }
  else
  {
    qDebug("using default language");
  }

#ifdef Q_OS_WIN32
  engine_.addImportPath("./qml/");
#endif
  root_window_ = (QQuickWindow*) engine_.load(QUrl("qrc:/main.qml"));

  // connect quit signal
  connect(&engine_, SIGNAL(quit()), this, SLOT(quit()));
  connect(root_window_, SIGNAL(visibleChanged(bool)), this, SLOT(quitOnHide(bool)));

  // show and connect tray icon
  tray_icon_.show();
  fillTrayIconMenu();
  connect(&tray_icon_, SIGNAL(activated(QSystemTrayIcon::ActivationReason)), this, SLOT(iconActivated(QSystemTrayIcon::ActivationReason)));

  // connect model
  connect(&site_model_, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(modelChanged()));
  connect(&site_model_, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(modelChanged()));
  connect(&site_model_, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(modelChanged()));

  // connect seettings
  connect(&settings_, SIGNAL(valueChanged(QString const &,QVariant const &)), this, SLOT(settingChanged(QString const &,QVariant const &)));

  // connect timers
  logout_timer_.setSingleShot(true);
  connect(&logout_timer_, SIGNAL(timeout()), this, SLOT(autoLogout()));
  connect(&bg_timer_, SIGNAL(timeout()), this, SLOT(checkForMessage()));

#ifdef Q_OS_WIN32
  // windows: clipboard::setText needs to be called from the main thread
  connect(&clipboard_thread_, SIGNAL(finished()), this, SLOT(resetClipboard()));
#endif
}

MasterPasswordApp::~MasterPasswordApp()
{
  save();
  if (!settings_.group().isEmpty())
    settings_.endGroup();
  settings_.setValue("window_width", root_window_->width());
  settings_.setValue("window_height", root_window_->height());

  if (clipboard_thread_.isRunning())
  {
    clipboard_thread_.clearNow();
    clipboard_thread_.blockUntilFinished();
  }

  resetSharedMemory();
}

bool MasterPasswordApp::isRunning()
{
  // attach() will return true if another instance is running
  QSharedMemory shared_mem;
  shared_mem.setKey(SHARED_MEM_KEY);
  bool running = shared_mem.attach();
  if (running)
    shared_mem.detach();
  return running;
}

int MasterPasswordApp::show()
{
  // write '1' to the shared memory to notify the running instance
  QSharedMemory shared_mem;
  shared_mem.setKey(SHARED_MEM_KEY);
  if (shared_mem.attach())
  {
    shared_mem.lock();
    int *show = (int*) shared_mem.data();
    *show = 1;
    shared_mem.unlock();
    shared_mem.detach();
    return 0;
  }
  return 1;
}

void MasterPasswordApp::resetSharedMemory()
{
  shared_mem_.lock();
  int *show = (int*) shared_mem_.data();
  *show = 0;
  shared_mem_.unlock();
}

void MasterPasswordApp::checkForMessage()
{
  shared_mem_.lock();
  int *show = (int*) shared_mem_.data();
  if (*show)
    root_window_->show();
  *show = 0;
  shared_mem_.unlock();
}

void MasterPasswordApp::fillTrayIconMenu()
{
  tray_icon_menu_.clear();

  if (loggedIn())
  {
    QMenu *sites_menu = new QMenu(tr("Sites"), &tray_icon_menu_);

    for (int i=0; i < site_model_.length(); i++)
    {
      SiteMenu *menu = new SiteMenu(&site_model_.siteAt(i), sites_menu);
      connect(menu, SIGNAL(triggered(QAction*)), this, SLOT(traySiteClicked(QAction*)));
      sites_menu->addMenu(menu);
    }

    tray_icon_menu_.addMenu(sites_menu);
  }

  hide_action_ = tray_icon_menu_.addAction(tr("&Hide"));
  show_action_ = tray_icon_menu_.addAction(tr("&Show"));
  QAction *quit_action = tray_icon_menu_.addAction(tr("&Quit"));

  show_action_->setVisible(false);

  connect(hide_action_, SIGNAL(triggered()), root_window_, SLOT(hide()));
  connect(show_action_, SIGNAL(triggered()), root_window_, SLOT(show()));
  connect(quit_action, SIGNAL(triggered()), this, SLOT(quit()));

  tray_icon_.setContextMenu(&tray_icon_menu_);
}

void MasterPasswordApp::autoLogout()
{
  // if the main window is visible then wait until it's closed
  if (root_window_->isVisible())
  {
    qDebug("delay logout");
    connect(root_window_, SIGNAL(visibleChanged(bool)), this, SLOT(autoLogout()));
    return;
  }
  qDebug("logging out automatically");
  disconnect(root_window_, SIGNAL(visibleChanged(bool)), this, SLOT(autoLogout()));
  emit requestLogout();
}

void MasterPasswordApp::iconActivated(QSystemTrayIcon::ActivationReason reason)
{
  if (reason == QSystemTrayIcon::DoubleClick)
    root_window_->setVisible(!root_window_->isVisible());
}

void MasterPasswordApp::traySiteClicked(QAction *action)
{
  SiteMenu *menu = (SiteMenu*) QObject::sender();
  Site const *site = menu->site();
  MPSiteVariant variant = action->data().value<MPSiteVariant>();
  QString password = master_key_->passwordForSite(*site, variant);
  copyToClipboard(password);
  tray_icon_.showMessage(site->name(),
                         tr("Copied password for %1 seconds").arg(settings_.clipboardDuration()),
                         QSystemTrayIcon::NoIcon,
                         2000);
}

void MasterPasswordApp::quitOnHide(bool isVisible)
{
  if (!isVisible)
    quit();
}

void MasterPasswordApp::toggleHideShow(bool isVisible)
{
  hide_action_->setVisible(isVisible);
  show_action_->setVisible(!isVisible);

  if (isVisible)
  {
    bg_timer_.stop();
  }
  else
  {
    resetSharedMemory();
    bg_timer_.start(LOGOUT_TIMER_INTERVAL);
    if (!settings_.trayInfoShown())
    {
      tray_icon_.showMessage(applicationName(), tr("I'm now running in the background!"));
      settings_.setTrayInfoShown(true);
    }
  }
}

void MasterPasswordApp::modelChanged()
{
  fillTrayIconMenu();
}

void MasterPasswordApp::settingChanged(QString const &key, QVariant const &value)
{
  qDebug() << "new value for" << key << ":" << value;
  if (key == key::algorithmVersion)
  {
    if (value.toUInt() != master_key_->algorithmVersion())
      emit requestLogout();
  }
  if (key == key::clipboardDuration)
  {
    if (clipboard_thread_.isRunning())
    {
      clipboard_thread_.clearNow();
      clipboard_thread_.blockUntilFinished();
    }
    clipboard_thread_.setTimout(value.toUInt());
    engine_.rootContext()->setContextProperty("ClipboardDuration", clipboard_thread_.timeout());
  }
  if (key == key::hidePasswords)
  {
    engine_.rootContext()->setContextProperty("HidePasswords", value);
  }
}

QString MasterPasswordApp::hash(QString const &str)
{
  return QString::fromUtf8(mpw_idForBuf(str.toStdString().c_str(), str.length()));
}

bool MasterPasswordApp::verifyPassword(QString const &password)
{
  return hash(password) == settings_.passwordHash();
}

QString MasterPasswordApp::passwordForSite(QString const &site_name, int site_type, int site_counter,
                                           QString const &site_variant, QString const &site_context)
{
  if (!loggedIn())
    return "master key not set";
  return master_key_->passwordForSite(site_name.toStdString().c_str(), site_type, site_counter,
                                      mpw::variantWithName(site_variant), site_context == "" ? NULL : site_context.toStdString().c_str());
}

void MasterPasswordApp::copyToClipboard(QString const &text)
{
  if (clipboard_thread_.isRunning())
  {
    clipboard_thread_.clearNow();
    clipboard_thread_.blockUntilFinished();
  }
  qDebug("copy to clipboard");
  QApplication::clipboard()->setText(text);
  clipboard_thread_.start();
}

QString MasterPasswordApp::obscure(QString text)
{
  // replace any character except whitespaces with '*'
  // \S matche a non-whitespace character
  return text.replace(QRegExp("\\S"), "*");
}

bool MasterPasswordApp::login(QString const &name, QString const &password)
{
  settings_.setUserName(name);
  settings_.beginGroup(name);

  if (settings_.verifyPassword() && !verifyPassword(password))
  {
    settings_.endGroup();
    return false;
  }

  master_key_.reset(new MasterKey(name.toStdString().c_str(), password.toStdString().c_str(), settings_.algorithmVersion()));

  QQmlContext *ctx = engine_.rootContext();

  // load site model from settings
  site_model_.load(settings_);
  // create other models based on site model
  site_proxy_model_ = new SiteProxyModel(&site_model_);
  category_model_ = new CategoryModel(&site_model_);
  site_history_model_ = new SiteHistoryModel(&site_model_);
  // load the models into QML
  ctx->setContextProperty("SiteProxyModel", site_proxy_model_);
  ctx->setContextProperty("CategoryModel", category_model_);
  ctx->setContextProperty("SiteHistoryModel", site_history_model_);

  ctx->setContextProperty("HidePasswords", settings_.hidePasswords());
  ctx->setContextProperty("ClipboardDuration", settings_.clipboardDuration());
  clipboard_thread_.setTimout(settings_.clipboardDuration());

  if (settings_.autoLogout())
    logout_timer_.start(settings_.autoLogoutDuration() * 60 * 1000);

  fillTrayIconMenu();

  disconnect(root_window_, SIGNAL(visibleChanged(bool)), this, SLOT(quitOnHide(bool)));
  connect(root_window_, SIGNAL(visibleChanged(bool)), this, SLOT(toggleHideShow(bool)));

  return true;
}

void MasterPasswordApp::logout()
{
  master_key_.reset(nullptr);

  // delete models which depend on site model
  delete site_proxy_model_;
  delete category_model_;
  delete site_history_model_;

  save();
  settings_.endGroup();

  fillTrayIconMenu();

  disconnect(root_window_, SIGNAL(visibleChanged(bool)), this, SLOT(toggleHideShow(bool)));
  connect(root_window_, SIGNAL(visibleChanged(bool)), this, SLOT(quitOnHide(bool)));
}

bool MasterPasswordApp::loggedIn()
{
  return master_key_;
}

void MasterPasswordApp::save()
{
  site_model_.save(settings_);
}

bool MasterPasswordApp::existsFile(QString const &path)
{
  return QFile(QUrl(path).toLocalFile()).exists();
}

bool MasterPasswordApp::existsDir(QString const &path)
{
  return QDir(QUrl(path).toLocalFile()).exists();
}

QString MasterPasswordApp::exportDirectory()
{
  return QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
}

void MasterPasswordApp::importFile(QString const &path, bool overwrite)
{
  QFile file(QUrl(path).toLocalFile());
  QFileInfo info(file);
  if (info.suffix() == "json")
  {
    site_model_.import(Importer::parseJson(file), overwrite);
  }
  save();
}

void MasterPasswordApp::exportFile(QString const &path)
{
  QFile file(QUrl(path).toLocalFile());
  QFileInfo info(file);
  if (info.suffix() == "json")
  {
    Exporter::toJson(site_model_.list(), settings_.algorithmVersion(), file);
  }
}

