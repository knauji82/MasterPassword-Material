/**
 * sitemenu.h
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

#ifndef SITEMENU
#define SITEMENU

#include "site.h"

#include <QMenu>

class SiteMenu : public QMenu
{
  Q_OBJECT

public:
  SiteMenu(Site const *site, QWidget *parent)
      : QMenu(site->name(), parent), site_(site)
  {
    if (site->password()->contentType() != Content::None)
      addAction(tr("Password"))->setData(MPSiteVariantPassword);
    if (site->login()->contentType() != Content::None)
      addAction(tr("Login"))->setData(MPSiteVariantLogin);
    if (site->answer()->contentType() != Content::None)
      addAction(tr("Answer"))->setData(MPSiteVariantAnswer);
  }

  inline Site const * site() const { return site_; }

private:
  Site const *site_;
};

#endif // SITEMENU

