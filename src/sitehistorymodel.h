/**
 * sitehistorymodel.h
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

#ifndef SITEHISTORYMODEL_H
#define SITEHISTORYMODEL_H

#define MAX_HISTORY 5

#include "sitemodel.h"

#include <QSortFilterProxyModel>

class SiteHistoryModel : public QSortFilterProxyModel
{
  Q_OBJECT

public:
  SiteHistoryModel(SiteModel *model);

  bool filterAcceptsRow(int row, QModelIndex const &parent) const;

  Q_INVOKABLE inline int mapIndexToSource(int index) const
  {
    return mapToSource(this->index(index, 0)).row();
  }

protected slots:
  void updateModel();

private:
  QDateTime min_date_;
};

#endif // SITEHISTORYMODEL_H
