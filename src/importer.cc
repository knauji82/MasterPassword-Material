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

    QJsonValue j_site_type = site.value("passwordType");
    MPSiteType site_type;
    if (j_site_type.toString() == "GeneratedMaximum")
      site_type = MPSiteTypeGeneratedMaximum;
    else if (j_site_type.toString() == "GeneratedLong")
      site_type = MPSiteTypeGeneratedLong;
    else if (j_site_type.toString() == "GeneratedMedium")
      site_type = MPSiteTypeGeneratedMedium;
    else if (j_site_type.toString() == "GeneratedShort")
      site_type = MPSiteTypeGeneratedShort;
    else if (j_site_type.toString() == "GeneratedBasic")
      site_type = MPSiteTypeGeneratedBasic;
    else if (j_site_type.toString() == "GeneratedPIN")
      site_type = MPSiteTypeGeneratedPIN;
    else if (j_site_type.toString() == "GeneratedName")
      site_type = MPSiteTypeGeneratedName;
    else if (j_site_type.toString() == "GeneratedPhrase")
      site_type = MPSiteTypeGeneratedPhrase;

    out << Site(site.value("siteName").toString(),
                site_type,
                site.value("siteCounter").toInt(),
                site.value("siteContext").isUndefined() ? "" : site.value("siteContext").toString(),
                QDateTime::fromString(site.value("lastUsed").toString(), "MMM d, yyyy h:mm:ss AP").toTime_t(),
                site.value("category").toString().isEmpty() ? QStringList() : QStringList(site.value("category").toString())
           );
  }
  file.close();
  return out;
}


