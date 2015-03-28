/**
 * exporter.cc
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

#include "exporter.h"

void Exporter::toJson(QList<Site> const &sites, MPAlgorithmVersion algorithm_version, QFile &file)
{
  QJsonArray site_array;

  for (QList<Site>::const_iterator i=sites.begin(); i != sites.end(); i++)
  {
    QJsonObject site_obj;

    QString type_name;

    switch (i->type())
    {
      case MPSiteTypeGeneratedMaximum:
        type_name = "GeneratedMaximum";
        break;
      case MPSiteTypeGeneratedLong:
        type_name = "GeneratedLong";
        break;
      case MPSiteTypeGeneratedMedium:
        type_name = "GeneratedMedium";
        break;
      case MPSiteTypeGeneratedShort:
        type_name = "GeneratedShort";
        break;
      case MPSiteTypeGeneratedBasic:
        type_name = "GeneratedBasic";
        break;
      case MPSiteTypeGeneratedPIN:
        type_name = "GeneratedPIN";
        break;
      case MPSiteTypeGeneratedName:
        type_name = "GeneratedName";
        break;
      case MPSiteTypeGeneratedPhrase:
        type_name = "GeneratedPhrase";
        break;
    }

    site_obj.insert("algorithmVersion", "V"+QString::number(algorithm_version));
    site_obj.insert("category", i->category());
    site_obj.insert("passwordType", type_name);
    site_obj.insert("siteName", i->name());
    site_obj.insert("siteCounter", i->counter());
    site_obj.insert("siteContext", i->context());
    if (i->lastUsed() > 0)
      site_obj.insert("lastUsed", QDateTime::fromTime_t(i->lastUsed()).toString("MMM d, yyyy h:mm:ss AP"));

    site_array.append(site_obj);
  }

  QJsonDocument doc;
  doc.setArray(site_array);

  file.open(QIODevice::WriteOnly | QIODevice::Text);
  QTextStream out(&file);

  out << doc.toJson();

  file.close();
}

