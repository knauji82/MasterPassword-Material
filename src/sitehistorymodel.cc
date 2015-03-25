/**
 * sitehistorymodel.cc
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

#include "sitehistorymodel.h"

SiteHistoryModel::SiteHistoryModel(SiteModel *model)
    : QSortFilterProxyModel(model)
{
  setSourceModel(model);
  setSortRole(SiteModel::LastUsedRole);
  connect(model, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(updateModel()));
  connect(model, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(updateModel()));
  connect(model, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateModel()));

  sort(0, Qt::DescendingOrder);
  updateModel();
}

void SiteHistoryModel::updateModel()
{
  min_date_ = data(index(std::min(MAX_HISTORY-1, rowCount()-1), 0), SiteModel::LastUsedRole).toDateTime();
  filterChanged();
}


bool SiteHistoryModel::filterAcceptsRow(int row, QModelIndex const &parent) const
{
  QDateTime date = sourceModel()->data(sourceModel()->index(row, 0, parent), SiteModel::LastUsedRole).toDateTime();
  QDateTime empty;

  if (min_date_ == empty && date == empty)
    return false;

  return  date >= min_date_;
}
