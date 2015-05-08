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
  roles[CategoryRole] = "siteCategory";
  roles[LastUsedRole] = "siteLastUsed";
  roles[LastVariantRole] = "siteLastVariant";
  roles[PasswordRole] = "sitePassword";
  roles[LoginRole] = "siteLogin";
  roles[AnswerRole] = "siteAnswer";
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

void SiteModel::updateDate(QModelIndex const &index, MPSiteVariant variant)
{
  Site *site = &sites_[index.row()];
  unsigned int time = QDateTime::currentDateTime().toTime_t();
  site->setLastUsed(time, variant);
  emit dataChanged(index, index);
}

void SiteModel::increaseCounter(QModelIndex const &index, MPSiteVariant variant)
{
  Site site = sites_[index.row()];

  switch (variant)
  {
    case MPSiteVariantPassword:
    {
      Q_ASSERT(site.password()->isGenerated());
      GeneratedPassword *pw = (GeneratedPassword*) site.password();
      pw->setCounter(pw->counter()+1);
      break;
    }
    case MPSiteVariantLogin:
    {
      Q_ASSERT(site.login()->isGenerated());
      GeneratedLogin *login = (GeneratedLogin*) site.login();
      login->setCounter(login->counter()+1);
      break;
    }
    case MPSiteVariantAnswer:
    {
      Q_ASSERT(site.answer()->isGenerated());
      GeneratedAnswer *answer = (GeneratedAnswer*) site.answer();
      answer->setCounter(answer->counter()+1);
      break;
    }
  }

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
    case CategoryRole:
      return site.category();
    case LastUsedRole:
      return QDateTime::fromTime_t(site.lastUsed());
    case LastVariantRole:
      return mpw::variantName(site.lastVariant());
    case PasswordRole:
      return site.password()->toMap();
    case LoginRole:
      return site.login()->toMap();
    case AnswerRole:
      return site.answer()->toMap();
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
