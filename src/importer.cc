/**
 * importer.cc
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

#include "importer.h"

QList<Site> Importer::parseJson(QFile &file)
{
  file.open(QIODevice::ReadOnly | QIODevice::Text);
  QList<Site> out;

  QJsonDocument doc = QJsonDocument::fromJson(file.readAll());

  QJsonArray sites = doc.array();
  for (QJsonArray::iterator i=sites.begin(); i != sites.end(); i++)
  {
    QJsonObject site = i->toObject();

    Content *password;
    Content *login;
    Content *answer;
    QString url;
    uint last_used;
    MPSiteVariant last_variant = 0;

    if (site.contains("passwordType") && site.contains("siteCounter"))
    {
      QJsonValue type_name = site.value("passwordType").toString();
      MPSiteType type;

      if (type_name == "GeneratedMaximum")
        type = MPSiteTypeGeneratedMaximum;
      else if (type_name == "GeneratedLong")
        type = MPSiteTypeGeneratedLong;
      else if (type_name == "GeneratedMedium")
        type = MPSiteTypeGeneratedMedium;
      else if (type_name == "GeneratedShort")
        type = MPSiteTypeGeneratedShort;
      else if (type_name == "GeneratedBasic")
        type = MPSiteTypeGeneratedBasic;
      else if (type_name == "GeneratedPIN")
        type = MPSiteTypeGeneratedPIN;
      else if (type_name == "GeneratedName")
        type = MPSiteTypeGeneratedName;
      else if (type_name == "GeneratedPhrase")
        type = MPSiteTypeGeneratedPhrase;

      password = new GeneratedPassword(type, site.value("siteCounter").toInt());
      login = new Content();
      answer = new Content();

      last_used = QDateTime::fromString(site.value("lastUsed").toString(), "MMM d, yyyy h:mm:ss AP").toTime_t();
    }
    else
    {
      password = passwordFromMap(site.value("password").toObject().toVariantMap());
      login = loginFromMap(site.value("login").toObject().toVariantMap());
      answer = answerFromMap(site.value("answer").toObject().toVariantMap());

      url = site.value("url").toString();
      last_used = site.value("lastUsed").toVariant().toUInt();
      last_variant = site.value("lastVariant").toInt();
    }

    out << Site(site.contains("siteName") ? site.value("siteName").toString() : site.value("name").toString(),
                password,
                login,
                answer,
                site.value("category").toString(),
                url,
                last_used,
                last_variant
           );
  }

  file.close();

  return out;
}


