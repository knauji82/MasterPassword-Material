/**
 * masterkey.h
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

#ifndef MASTERKEY_H
#define MASTERKEY_H

#include "mpw.h"
#include "site.h"
#include "simplecrypt.h"

class MasterKey
{
private:
  MPAlgorithmVersion algorithm_version_;
  uint8_t const *key_;

public:
  MasterKey(char const *full_name, char const *master_password)
      : MasterKey(full_name, master_password, MPAlgorithmVersionCurrent) {}
  MasterKey(char const *full_name, char const *master_password, MPAlgorithmVersion algorithm_version)
      : algorithm_version_(algorithm_version), key_(mpw_masterKeyForUser(full_name, master_password, algorithm_version)) {}

  ~MasterKey()
  {
    mpw_free(key_, MP_dkLen);
  }

  inline MPAlgorithmVersion algorithmVersion() const
  {
    return algorithm_version_;
  }

  inline char const * passwordForSite(Site const &site, MPSiteVariant const site_variant)
  {
    return passwordForSite(site.name().toStdString().c_str(), site.type(), site.counter(), site_variant,
                           site.context().isEmpty() ? NULL : site.context().toStdString().c_str());
  }

  inline char const * passwordForSite(char const *site_name, MPSiteType const site_type, uint32_t const site_counter, MPSiteVariant const site_variant, char const *site_context)
  {
    return mpw_passwordForSite(key_, site_name, site_type, site_counter, site_variant, site_context, algorithm_version_);
  }

  QString encrypt(QString const &text);

  QString decrypt(QString const &encrypted);
};

#endif // MASTERKEY_H

