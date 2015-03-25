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

int SiteProxyModel::insert(QString const &name, QString const &type_name, int counter, QString const &context, QString const &category, bool overwrite)
{
  int index = site_model_->insert(name, type_name, counter, context, category, overwrite);
  return mapFromSource(site_model_->index(index)).row();
}

int SiteProxyModel::modify(int index, QString const &name, QString const &type_name, int counter, QString const &context, QString const &category)
{
  bool name_changed = name != siteAt(index).name();
  if (name_changed)
    remove(index);

  return insert(name, type_name, counter, context, category, !name_changed);
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
