/**
 * site.h
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

#ifndef SITE
#define SITE

#include <QSharedPointer>

#include "content.h"

class Site
{
public:
  Site(QString const &name, Content *password, Content *login, Content *answer, QString const &category=QString(),
       QString const &url=QString(), uint last_used=0, MPSiteVariant last_variant=0)
      : name_(name),
        category_(category),
        url_(url),
        last_used_(last_used),
        last_variant_(last_variant)
  {
    password_.reset(password);
    login_.reset(login);
    answer_.reset(answer);
  }

  inline QString const & name() const { return name_; }
  inline void setName(QString const &name) { name_ = name; }

  inline QString const & category() const { return category_; }
  inline void setCategory(QString const &category) { category_ = category; }

  inline QString const & url() const { return url_; }
  inline void setUrl(QString const &url) { url_ = url; }

  inline uint lastUsed() const { return last_used_; }
  inline MPSiteVariant lastVariant() const { return last_variant_; }
  inline void setLastUsed(uint last_used, MPSiteVariant variant)
  {
    last_used_ = last_used;
    last_variant_ = variant;
  }

  inline Content * password() const { return password_.data(); }
  inline Content * login() const { return login_.data(); }
  inline Content * answer() const { return answer_.data(); }

private:
  QString name_;
  QString category_;
  QString url_;
  uint last_used_;
  MPSiteVariant last_variant_;

  QSharedPointer<Content> password_;
  QSharedPointer<Content> login_;
  QSharedPointer<Content> answer_;
};

#endif // SITE

