/**
 * categorymodel.h
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

#ifndef CATEGORYMODEL_H
#define CATEGORYMODEL_H

#include "sitemodel.h"

#include <QStringListModel>

class CategoryModel : public QStringListModel
{
  Q_OBJECT

private:
  SiteModel *site_model_;

public:
  CategoryModel(SiteModel *model);

  QHash<int, QByteArray> roleNames() const;

public slots:
  void updateCategories();

signals:
  void modelChanged();
};

#endif // CATEGORYMODEL_H
