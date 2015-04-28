/**
 * masterpasswordapp.h
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

#ifndef MASTERPASSWORDAPP_H
#define MASTERPASSWORDAPP_H

#include "masterkey.h"
#include "qmlapplicationengine.h"
#include "settings.h"
#include "sitemodel.h"
#include "siteproxymodel.h"
#include "categorymodel.h"
#include "sitehistorymodel.h"
#include "clipboard.h"

#include <QApplication>
#include <QQuickWindow>
#include <QSystemTrayIcon>
#include <QMenu>

class MasterPasswordApp : public QApplication
{
  Q_OBJECT

private:
  QmlApplicationEngine engine_;
  QQuickWindow *root_window_;

  Settings settings_;

  QScopedPointer<MasterKey> master_key_;

  SiteModel site_model_;
  SiteProxyModel   *site_proxy_model_;
  CategoryModel    *category_model_;
  SiteHistoryModel *site_history_model_;

  ResetClipboardThread clipboard_thread_;

  QSharedMemory shared_mem_;
  QTimer bg_timer_;
  QTimer logout_timer_;

  QTranslator translator_;

  QSystemTrayIcon tray_icon_;
  QMenu tray_icon_menu_;
  QAction *hide_action_;
  QAction *show_action_;

protected:
  void resetSharedMemory();
  void fillTrayIconMenu();

protected slots:
  void checkForMessage();
  void autoLogout();
  void iconActivated(QSystemTrayIcon::ActivationReason reason);
  void traySiteClicked(QAction *action);
  void quitOnHide(bool visible);
  void toggleHideShow(bool visible);
  void modelChanged();
  void settingChanged(QString const &key, QVariant const &value);
#ifdef Q_OS_WIN32
  inline void resetClipboard() {
    QApplication::clipboard()->setText("");
  }
#endif

public:
  MasterPasswordApp(int &argc, char *argv[]);
  ~MasterPasswordApp();

  static bool isRunning();

  static int show();

  Q_INVOKABLE QString hash(QString const &str);

  bool verifyPassword(QString const &password);

  Q_INVOKABLE QString passwordForSite(QString const &site_name, int site_type, int site_counter,
                                      QString const &site_variant, QString const &site_context="");

  Q_INVOKABLE QString encrypt(QString const &text);

  Q_INVOKABLE QString decrypt(QString const &text);

  Q_INVOKABLE void copyToClipboard(QString const &text);

  Q_INVOKABLE QString obscure(QString text);

  Q_INVOKABLE bool login(QString const &name, QString const &password);

  Q_INVOKABLE void logout();

  Q_INVOKABLE bool loggedIn();

  Q_INVOKABLE void save();

  Q_INVOKABLE bool existsFile(QString const &path);

  Q_INVOKABLE bool existsDir(QString const &path);

  Q_INVOKABLE QString exportDirectory();

  Q_INVOKABLE void importFile(QString const &path, bool overwrite);

  Q_INVOKABLE void exportFile(QString const &path, QVariantMap const &args);

signals:
  void requestLogout();
};

#endif // MASTERPASSWORDAPP_H
