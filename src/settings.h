/**
 * settings.h
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

#ifndef SETTINGS_H
#define SETTINGS_H

#include "mpw.h"
#include "site.h"

#include <QSettings>

#define SETTINGS_FILE "settings"

namespace key
{
  QString const appVersion = "app_version";

  QString const algorithmVersion    = "algorithm_version";
  QString const userName            = "user_name";
  QString const defaultSiteType     = "default_site_type";
  QString const clipboardDuration   = "clipboard_duration";
  QString const autoLogout          = "auto_logout";
  QString const autoLogoutDuration  = "auto_logout_duration";
  QString const hidePasswords       = "hide_passwords";
  QString const verifyPassword      = "verify_password";
  QString const passwordHash        = "password_hash";
  QString const trayInfoShown       = "tray_info_shown";
  QString const sortOrder           = "sort_order";

  namespace site
  {
    QString const name        = "name";
    QString const category    = "category";
    QString const url         = "url";
    QString const lastUsed    = "last_used";
    QString const lastVariant = "last_variant";
    QString const type        = "type";
    QString const counter     = "counter";
    QString const context     = "context";
    QString const content     = "content";
    QString const contentType = "content_type";
    QString const password    = "password";
    QString const login       = "login";
    QString const answer      = "answer";
  }
}

class Settings : public QSettings
{
  Q_OBJECT

public:
  explicit Settings(QObject *parent=0) : QSettings(QSettings::NativeFormat,
                                                   QSettings::UserScope,
                                                   QCoreApplication::applicationName(),
                                                   SETTINGS_FILE,
                                                   parent) {}

  Q_INVOKABLE inline QVariant value(QString const &key, QVariant const &default_value=QVariant()) const
  {
    return QSettings::value(key, default_value);
  }

  template <typename T>
  inline T value(QString const &key, QVariant const &default_value=QVariant()) const
  {
    return value(key, default_value).value<T>();
  }

  Q_INVOKABLE void setValue(QString const &key, const QVariant &value)
  {
    if (value != this->value(key))
    {
      QSettings::setValue(key, value);
      emit valueChanged(key, value);
    }
  }

  inline QString appVersion() const { return value<QString>(key::appVersion, "1.0.0"); }
  inline void setAppVersion(QString const &v) { setValue(key::appVersion, v); }

  Q_INVOKABLE inline uint algorithmVersion() const { return value<uint>(key::algorithmVersion, MPAlgorithmVersionCurrent); }
  Q_INVOKABLE inline void setAlgorithmVersion(uint v) { setValue(key::algorithmVersion, v); }

  Q_INVOKABLE inline QString userName() const { return value<QString>(key::userName); }
  Q_INVOKABLE inline void setUserName(QString const &v) { setValue(key::userName, v); }

  Q_INVOKABLE inline uint defaultSiteType() const { return value<uint>(key::defaultSiteType, MPSiteTypeGeneratedLong); }
  Q_INVOKABLE inline void setDefaultSiteType(uint v) { setValue(key::defaultSiteType, v); }

  Q_INVOKABLE inline uint clipboardDuration() const { return value<uint>(key::clipboardDuration, 20); }
  Q_INVOKABLE inline void setClipboardDuration(uint v) { setValue(key::clipboardDuration, v); }

  Q_INVOKABLE inline bool autoLogout() const { return value<bool>(key::autoLogout, true); }
  Q_INVOKABLE inline void setAutoLogout(bool v) { setValue(key::autoLogout, v); }

  Q_INVOKABLE inline uint autoLogoutDuration() const { return value<uint>(key::autoLogoutDuration, 20); }
  Q_INVOKABLE inline void setAutoLogoutDuration(uint v) { setValue(key::autoLogoutDuration, v); }

  Q_INVOKABLE inline bool hidePasswords() const { return value<bool>(key::hidePasswords, false); }
  Q_INVOKABLE inline void setHidePasswords(bool v) { setValue(key::hidePasswords, v); }

  Q_INVOKABLE inline bool verifyPassword() const { return value<bool>(key::verifyPassword, false); }
  Q_INVOKABLE inline void setVerifyPassword(bool v) { setValue(key::verifyPassword, v); }

  Q_INVOKABLE inline QString passwordHash() const { return value<QString>(key::passwordHash); }
  Q_INVOKABLE inline void setPasswordHash(QString const &v) { setValue(key::passwordHash, v); }

  Q_INVOKABLE inline bool trayInfoShown() const { return value<bool>(key::trayInfoShown, false); }
  Q_INVOKABLE inline void setTrayInfoShown(bool v) { setValue(key::trayInfoShown, v); }

  Q_INVOKABLE inline int sortOrder() const { return value<int>(key::sortOrder, Qt::AscendingOrder); }
  Q_INVOKABLE inline void setSortOrder(int v) { setValue(key::sortOrder, v); }

  void store(Site const &site);
  Site loadSite();
  void removeSite();

signals:
  void valueChanged(QString const &key, const QVariant &value);
};

Q_DECLARE_METATYPE(Settings*)

#endif // SETTINGS_H
