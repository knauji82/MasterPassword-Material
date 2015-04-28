/**
 * siteproxymodel.cc
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

#include "siteproxymodel.h"

SiteProxyModel::SiteProxyModel(SiteModel *model)
    : QSortFilterProxyModel(model), site_model_(model)
{
  setSourceModel(site_model_);
  setDynamicSortFilter(true);
  setSortRole(SiteModel::NameRole);
  setSortCaseSensitivity(Qt::CaseInsensitive);
  setFilterRole(SiteModel::CategoryRole);
  setFilterCaseSensitivity(Qt::CaseSensitive);
}

int SiteProxyModel::insert(QVariantMap const &site_map, bool overwrite)
{
  Content *password = passwordFromMap(site_map["password"].toMap());
  Content *login = loginFromMap(site_map["login"].toMap());
  Content *answer = answerFromMap(site_map["answer"].toMap());

  int index = site_model_->insert(Site(site_map["name"].toString(), password, login, answer, site_map["category"].toString(), site_map["lastUsed"].toUInt(),
      site_map["lastVariant"].toString().isEmpty() ? 0 : mpw::variantWithName(site_map["lastVariant"].toString())), overwrite);
  return mapFromSource(site_model_->index(index)).row();
}

int SiteProxyModel::modify(int index, QVariantMap const &site_map)
{
  bool name_changed = site_map["name"].toString() != siteAt(index).name();
  if (name_changed)
    remove(index);

  // make sure mapFromSource() will work
  if (site_map["category"].toString().isEmpty())
    setCategoryFilter("");

  return insert(site_map, !name_changed);
}

void SiteProxyModel::setCategoryFilter(QString const &category)
{
  if (!category.isEmpty())
    setCategoryFilter("");
  category_filter_ = category;
  filterChanged();
}

bool SiteProxyModel::filterAcceptsRow(int row, QModelIndex const &parent) const
{
  if (category_filter_.isEmpty())
    return true;

  QModelIndex index = this->index(row, 0, parent);

  if (!index.isValid())
    return true;

  Site const site = site_model_->list()[index.row()];

  return site.category() == category_filter_;
}
