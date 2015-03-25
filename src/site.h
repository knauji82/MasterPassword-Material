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

#include "mpw.h"

struct Site
{
private:
  QString      name_;
  MPSiteType   type_;
  int          counter_;
  QString      context_;
  unsigned int last_used_;
  QString      category_;

public:
  Site(QString const &name, MPSiteType type, int counter, QString const &context, uint last_used=0, QString const &category=QString())
      : name_(name),
        type_(type),
        counter_(counter),
        context_(context),
        last_used_(last_used),
        category_(category) {}

  inline QString const & name() const
  {
    return name_;
  }

  inline void setName(QString const &name)
  {
    name_ = name;
  }

  inline MPSiteType type() const
  {
    return type_;
  }

  inline int typeAsInt() const
  {
    return type_;
  }

  inline void setType(MPSiteType type)
  {
    type_ = type;
  }

  inline void setType(int type)
  {
    setType(static_cast<MPSiteType>(type));
  }

  inline int counter() const
  {
    return counter_;
  }

  inline void setCounter(int counter)
  {
    counter_ = counter;
  }

  inline QString const & context() const
  {
    return context_;
  }

  inline void setContext(QString const &context)
  {
    context_ = context;
  }

  inline uint lastUsed() const
  {
    return last_used_;
  }

  inline void setLastUsed(uint last_used)
  {
    last_used_ = last_used;
  }

  inline QString const & category() const
  {
    return category_;
  }

  inline void setCategory(QString const &category)
  {
    category_ = category;
  }
};

#endif // SITE

