/**
 * mpw.cc
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

#include "mpw.h"

QStringList mpw::algorithmVersions()
{
  QStringList versions;
  for (MPAlgorithmVersion v=MPAlgorithmVersion0; v <= MPAlgorithmVersionCurrent; v++)
    versions << QString::number(v);
  return versions;
}

QStringList mpw::typeNames()
{
  QStringList types;
  types << typeNameMaximumSecurityPassword()
        << typeNameLongPassword()
        << typeNameMediumPassword()
        << typeNameBasicPassword()
        << typeNameShortPassword()
        << typeNamePIN()
        << typeNameName()
        << typeNamePhrase();
  return types;
}

QStringList mpw::variantNames()
{
  QStringList variants;
  variants << variantNamePassword()
           << variantNameLogin()
           << variantNameAnswer();
  return variants;
}

QString mpw::typeName(MPSiteType type)
{
  switch (type)
  {
    case MPSiteTypeGeneratedMaximum:
      return typeNameMaximumSecurityPassword();
    case MPSiteTypeGeneratedLong:
      return typeNameLongPassword();
    case MPSiteTypeGeneratedMedium:
      return typeNameMediumPassword();
    case MPSiteTypeGeneratedBasic:
      return typeNameBasicPassword();
    case MPSiteTypeGeneratedShort:
      return typeNameShortPassword();
    case MPSiteTypeGeneratedPIN:
      return typeNamePIN();
    case MPSiteTypeGeneratedName:
      return typeNameName();
    case MPSiteTypeGeneratedPhrase:
      return typeNamePhrase();
    default:
      qFatal("Unkown type: %d", type);
  }
}

MPSiteType mpw::typeWithName(QString const &type_name)
{
  if (type_name == typeNameMaximumSecurityPassword())
    return MPSiteTypeGeneratedMaximum;
  if (type_name == typeNameLongPassword())
    return MPSiteTypeGeneratedLong;
  if (type_name == typeNameMediumPassword())
    return MPSiteTypeGeneratedMedium;
  if (type_name == typeNameBasicPassword())
    return MPSiteTypeGeneratedBasic;
  if (type_name == typeNameShortPassword())
    return MPSiteTypeGeneratedShort;
  if (type_name == typeNamePIN())
    return MPSiteTypeGeneratedPIN;
  if (type_name == typeNameName())
    return MPSiteTypeGeneratedName;
  if (type_name == typeNamePhrase())
    return MPSiteTypeGeneratedPhrase;

  qFatal("Unkown type: %s", type_name.toStdString().c_str());
}

QString mpw::variantName(MPSiteVariant variant)
{
  switch (variant)
  {
    case MPSiteVariantPassword:
      return variantNamePassword();
    case MPSiteVariantLogin:
      return variantNameLogin();
    case MPSiteVariantAnswer:
      return variantNameAnswer();
    default:
      qFatal("Unkown variant: %d", variant);
  }
}

MPSiteVariant mpw::variantWithName(QString const &variant_name)
{
  if (variant_name == variantNamePassword())
    return MPSiteVariantPassword;
  if (variant_name == variantNameLogin())
    return MPSiteVariantLogin;
  if (variant_name == variantNameAnswer())
    return MPSiteVariantAnswer;

  qFatal("Unkown variant: %s", variant_name.toStdString().c_str());
}
