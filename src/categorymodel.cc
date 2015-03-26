/**
 * categorymodel.cc
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

#include "categorymodel.h"

CategoryModel::CategoryModel(SiteModel *model)
    : QStringListModel(model), site_model_(model)
{
  connect(site_model_, SIGNAL(rowsInserted(QModelIndex,int,int)), this, SLOT(updateCategories()));
  connect(site_model_, SIGNAL(rowsRemoved(QModelIndex,int,int)), this, SLOT(updateCategories()));
  connect(site_model_, SIGNAL(dataChanged(QModelIndex,QModelIndex)), this, SLOT(updateCategories()));
  updateCategories();
}

QHash<int, QByteArray> CategoryModel::roleNames() const
{
  QHash<int, QByteArray> roles;
  roles[Qt::DisplayRole] = "modelData";
  return roles;
}

void CategoryModel::updateCategories()
{
  QStringList list;

  for (QList<Site>::const_iterator i=site_model_->list().begin(); i != site_model_->list().end(); i++)
    if (!i->category().isEmpty() && !list.contains(i->category()))
      list << i->category();

  list.sort(Qt::CaseInsensitive);
  list.prepend(tr("All"));
  setStringList(list);
  emit modelChanged();
}

