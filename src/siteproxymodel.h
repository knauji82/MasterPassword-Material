/**
 * siteproxymodel.h
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

#ifndef SITEPROXYMODEL_H
#define SITEPROXYMODEL_H

#include "sitemodel.h"

#include <QSortFilterProxyModel>

class SiteProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
  Q_PROPERTY(QString categoryFilter WRITE setCategoryFilter)

private:
  SiteModel *site_model_;
  QString category_filter_;

public:
  SiteProxyModel(SiteModel *model);

  Q_INVOKABLE void sort(Qt::SortOrder order=Qt::AscendingOrder)
  {
    if (order != sortOrder())
    {
      QSortFilterProxyModel::sort(0, order);
      emit sortOrderChanged(order);
    }
  }

  Q_INVOKABLE int insert(QVariantMap const &site_map, bool overwrite=false);

  Q_INVOKABLE int modify(int index, QVariantMap const &site_map);

  Q_INVOKABLE inline void updateDate(int index, QString const &variant_name)
  {
    site_model_->updateDate(mapToSource(this->index(index, 0)), mpw::variantWithName(variant_name));
  }

  Q_INVOKABLE inline void remove(int index)
  {
    removeRow(index);
  }

  void setCategoryFilter(QString const &category);

  bool filterAcceptsRow(int row, QModelIndex const &parent) const;

  inline Site const & siteAt(int index)
  {
    return site_model_->list()[mapToSource(this->index(index, 0)).row()];
  }

  Q_INVOKABLE inline int mapIndexToSource(int index) const
  {
    return mapToSource(this->index(index, 0)).row();
  }

  Q_INVOKABLE inline int mapIndexFromSource(int source_index) const
  {
    return mapFromSource(site_model_->index(source_index)).row();
  }

  inline QVariantMap getMap(QModelIndex const &index)
  {
    return site_model_->getMap(mapToSource(index));
  }

  Q_INVOKABLE inline QVariantMap getMap(int index)
  {
    return getMap(this->index(index, 0));
  }

  inline int length()
  {
    return rowCount();
  }

signals:
  void sortOrderChanged(Qt::SortOrder order);
};

#endif // SITEPROXYMODEL_H

