/**
 * mpw.h
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

#ifndef MPW_H
#define MPW_H

#ifndef MP_ALGO_H
#define MP_ALGO_H
extern "C" {
  #include "lib/masterpassword-algorithm/mpw-algorithm.h"
}
#endif
#ifndef MP_UTIL_H
#define MP_UTIL_H
extern "C" {
  #include "lib/masterpassword-algorithm/mpw-util.h"
}
#endif

#include <QtQml>

class mpw : public QObject
{
  Q_OBJECT

public:
  static QObject * qmlInstance(QQmlEngine *engine, QJSEngine *script_engine)
  {
    Q_UNUSED(engine);
    Q_UNUSED(script_engine);

    return new mpw;
  }

  static inline void registerQml()
  {
    qmlRegisterSingletonType<mpw>("MasterPassword", 1, 0, "MPW", &qmlInstance);
  }

  Q_INVOKABLE static inline QString typeNameMaximumSecurityPassword()
  {
    return tr("Maximum Security");
  }

  Q_INVOKABLE static inline QString typeNameLongPassword()
  {
    return tr("Long");
  }

  Q_INVOKABLE static inline QString typeNameMediumPassword()
  {
    return tr("Medium");
  }

  Q_INVOKABLE static inline QString typeNameBasicPassword()
  {
    return tr("Basic");
  }

  Q_INVOKABLE static inline QString typeNameShortPassword()
  {
    return tr("Short");
  }

  Q_INVOKABLE static inline QString typeNamePIN()
  {
    return tr("PIN");
  }

  Q_INVOKABLE static inline QString typeNameName()
  {
    return tr("Name");
  }

  Q_INVOKABLE static inline QString typeNamePhrase()
  {
    return tr("Phrase");
  }

  Q_INVOKABLE static inline QString variantNamePassword()
  {
    return tr("Password");
  }

  Q_INVOKABLE static inline QString variantNameLogin()
  {
    return tr("Login");
  }

  Q_INVOKABLE static inline QString variantNameAnswer()
  {
    return tr("Answer");
  }

  Q_INVOKABLE static QStringList algorithmVersions();

  Q_INVOKABLE static QStringList passwordTypeNames();

  Q_INVOKABLE static QStringList variantNames();

  static QString typeName(MPSiteType type);

  Q_INVOKABLE static inline QString typeName(int type)
  {
    return typeName(static_cast<MPSiteType>(type));
  }

  static MPSiteType typeWithName(QString const &type_name);

  Q_INVOKABLE static inline uint typeWithNameAsInt(QString const &type_name)
  {
    return typeWithName(type_name);
  }

  static QString variantName(MPSiteVariant variant);

  Q_INVOKABLE static inline QString variantName(int variant)
  {
    return variantName(static_cast<MPSiteVariant>(variant));
  }

  static MPSiteVariant variantWithName(QString const &variant_name);

  Q_INVOKABLE static inline uint variantWithNameAsInt(QString const &variant_name)
  {
    return variantWithName(variant_name);
  }
};

#endif // MPW_H
