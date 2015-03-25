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

    QJsonValue type_name = site.value("passwordType").toString();
    MPSiteType site_type;

    if (type_name == "GeneratedMaximum")
      site_type = MPSiteTypeGeneratedMaximum;
    else if (type_name == "GeneratedLong")
      site_type = MPSiteTypeGeneratedLong;
    else if (type_name == "GeneratedMedium")
      site_type = MPSiteTypeGeneratedMedium;
    else if (type_name == "GeneratedShort")
      site_type = MPSiteTypeGeneratedShort;
    else if (type_name == "GeneratedBasic")
      site_type = MPSiteTypeGeneratedBasic;
    else if (type_name == "GeneratedPIN")
      site_type = MPSiteTypeGeneratedPIN;
    else if (type_name == "GeneratedName")
      site_type = MPSiteTypeGeneratedName;
    else if (type_name == "GeneratedPhrase")
      site_type = MPSiteTypeGeneratedPhrase;

    out << Site(site.value("siteName").toString(),
                site_type,
                site.value("siteCounter").toInt(),
                site.value("siteContext").isUndefined() ? "" : site.value("siteContext").toString(),
                QDateTime::fromString(site.value("lastUsed").toString(), "MMM d, yyyy h:mm:ss AP").toTime_t(),
                site.value("category").toString()
           );
  }

  file.close();

  return out;
}


