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

