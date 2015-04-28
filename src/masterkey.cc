/**
 * masterkey.cc
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

#include "masterkey.h"

char const * MasterKey::passwordForSite(Site const &site, MPSiteVariant variant)
{
  switch (variant)
  {
    case MPSiteVariantPassword:
    {
      Q_ASSERT(site.password()->contentType() == Content::Generated);
      GeneratedPassword *pw = (GeneratedPassword*) site.password();
      return passwordForSite(site.name().toStdString().c_str(), pw->type(), pw->counter(), variant);
    }
    case MPSiteVariantLogin:
    {
      Q_ASSERT(site.login()->contentType() == Content::Generated);
      GeneratedLogin *login = (GeneratedLogin*) site.login();
      return passwordForSite(site.name().toStdString().c_str(), login->type(), login->counter(), variant);
    }
    case MPSiteVariantAnswer:
    {
      Q_ASSERT(site.answer()->contentType() == Content::Generated);
      GeneratedAnswer *answer = (GeneratedAnswer*) site.answer();
      return passwordForSite(site.name().toStdString().c_str(), answer->type(), answer->counter(), variant,
                             answer->context().isEmpty() ? NULL : answer->context().toStdString().c_str());
    }
  }
}

QString MasterKey::encrypt(QString const &text)
{
  SimpleCrypt crypto;
  qint64 key;
  QString encrypted = text;

  for (int i=0; i < MP_dkLen; i += sizeof(qint64))
  {
    memcpy(&key, key_+i, sizeof(qint64));
    crypto.setKey(key);
    encrypted = crypto.encryptToString(encrypted);
  }

  return encrypted;
}

QString MasterKey::decrypt(QString const &encrypted)
{
  SimpleCrypt crypto;
  qint64 key;
  QString decrypted = encrypted;

  for (int i=MP_dkLen-sizeof(qint64); i >= 0; i -= sizeof(qint64))
  {
    memcpy(&key, key_+i, sizeof(qint64));
    crypto.setKey(key);
    decrypted = crypto.decryptToString(decrypted);
  }

  return decrypted;
}
