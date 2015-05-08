/**
 * sitemodel.h
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

#ifndef SITEMODEL_H
#define SITEMODEL_H

#include "site.h"
#include "mpw.h"
#include "settings.h"

#include <QAbstractListModel>

class SiteModel : public QAbstractListModel
{
  Q_OBJECT

private:
  QList<Site> sites_;

  void append(Site const &site);
  int insert(Site const &site, int left, int right, bool overwrite);

protected:
  QHash<int, QByteArray> roleNames() const;

public:
  explicit SiteModel(QObject *parent=0);

  enum SiteRole {
    NameRole = Qt::UserRole + 1,
    CategoryRole,
    LastUsedRole,
    LastVariantRole,
    PasswordRole,
    LoginRole,
    AnswerRole
  };

  inline QList<Site> const & list() const
  {
    return sites_;
  }

  int insert(Site const &site, bool overwrite=false);

  void updateDate(QModelIndex const &index, MPSiteVariant variant);

  void increaseCounter(QModelIndex const &index, MPSiteVariant variant);

  bool removeRows(int row, int count, QModelIndex const &parent);

  inline int length() const
  {
    return sites_.length();
  }

  int rowCount(const QModelIndex &parent=QModelIndex()) const;

  inline Site const & siteAt(int index) const
  {
    return sites_.at(index);
  }

  QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;

  QVariantMap getMap(QModelIndex const &index) const;

  void import(QList<Site> const &sites, bool overwrite=false);

  void load(Settings &settings);

  void save(Settings &settings);
};

#endif // SITEMODEL_H
