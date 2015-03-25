/**
 * sitemodel.cc
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

#include "sitemodel.h"

SiteModel::SiteModel(QObject *parent) : QAbstractListModel(parent) {}

QHash<int, QByteArray> SiteModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[NameRole] = "siteName";
  roles[CounterRole] = "siteCounter";
  roles[TypeRole] = "siteType";
  roles[ContextRole] = "siteContext";
  roles[LastUsedRole] = "siteLastUsed";
  roles[CategoryRole] = "siteCategory";
  return roles;
}

void SiteModel::append(Site const &site)
{
  beginInsertRows(QModelIndex(), rowCount(), rowCount());
  sites_ << site;
  endInsertRows();
}

int SiteModel::insert(Site const &site, bool overwrite)
{
  return insert(site, 0, length()-1, overwrite);
}

int SiteModel::insert(Site const &site, int left, int right, bool overwrite)
{
  QString name;

  if (left > right)
  {
    beginInsertRows(QModelIndex(), left, left);
    sites_.insert(left, site);
    endInsertRows();
    return left;
  }
  if (left == right)
  {
    name = sites_[left].name().toUpper();
    int index = left;
    if (site.name().toUpper() < name)
    {

    }
    else if (site.name().toUpper() > name)
    {
      index = right+1;
    }
    else if (overwrite)
    {
      sites_.replace(index, site);
      QModelIndex model_index = this->index(index);
      emit dataChanged(model_index, model_index);
      return index;
    }
    else
    {
      return -1;
    }
    beginInsertRows(QModelIndex(), index, index);
    sites_.insert(index, site);
    endInsertRows();
    return index;
  }

  int mid = (left + right) / 2;
  name = sites_[mid].name().toUpper();

  if (site.name().toUpper() < name)
    return insert(site, left, mid-1, overwrite);
  else if (site.name().toUpper() > name)
    return insert(site, mid+1, right, overwrite);
  else if (overwrite)
    return insert(site, mid, mid, true);
  else
    return -1;
}

bool SiteModel::removeRows(int row, int count, QModelIndex const &parent)
{
  beginRemoveRows(parent, row, row+count-1);
  for (int i=row; i < row+count; i++)
    sites_.removeAt(i);
  endRemoveRows();
  return true;
}

void SiteModel::updateDate(QModelIndex const &index)
{
  Site *site = &sites_[index.row()];
  unsigned int time = QDateTime::currentDateTime().toTime_t();
  site->setLastUsed(time);
  emit dataChanged(index, index);
}

int SiteModel::rowCount(QModelIndex const &parent) const
{
  Q_UNUSED(parent);
  return sites_.count();
}

QVariant SiteModel::data(QModelIndex const &index, int role) const
{
  if (index.row() < 0 || index.row() >= sites_.count())
    return QVariant();

  Site const &site = sites_[index.row()];

  switch (role)
  {
    case NameRole:
      return site.name();
    case TypeRole:
      return site.typeAsInt();
    case CounterRole:
      return site.counter();
    case ContextRole:
      return site.context();
    case LastUsedRole:
     // return site.lastUsed() > 0 ? QDateTime::fromTime_t(site.lastUsed()).toString("dd.MM.yyyy hh:mm") : "";
      return QDateTime::fromTime_t(site.lastUsed());
    case CategoryRole:
      return site.category();
  }

  return QVariant();
}

QVariantMap SiteModel::getMap(QModelIndex const &index) const
{
  QVariantMap map;
  foreach (int k, roleNames().keys())
    map[roleNames().value(k)] = data(index, k);
  return map;
}

void SiteModel::import(QList<Site> const &sites, bool overwrite)
{
  for (int i=0; i < sites.length(); i++)
    insert(sites[i], overwrite);
}

void SiteModel::load(Settings &settings)
{
  sites_.clear();

  int size = settings.beginReadArray("sites");
  for (int i=0; i < size; i++)
  {
    settings.setArrayIndex(i);
    append(settings.loadSite());
  }
  settings.endArray();
}

void SiteModel::save(Settings &settings)
{
  int i;

  // get the number of currently saved sites
  int size = settings.beginReadArray("sites");
  settings.endArray();

  settings.beginWriteArray("sites", sites_.count());

  for (i=0; i < sites_.count(); i++)
  {
    settings.setArrayIndex(i);
    settings.store(sites_[i]);
  }
  // remove unused sites
  for (; i < size; i++)
  {
    settings.setArrayIndex(i);
    settings.removeSite();
  }

  settings.endArray();
}
