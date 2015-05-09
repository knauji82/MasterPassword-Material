/**
 * settings.cc
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

#include "settings.h"

void Settings::store(Site const &site)
{
  setValue(key::site::name, site.name());
  setValue(key::site::category, site.category());
  setValue(key::site::url, site.url());
  setValue(key::site::lastUsed, site.lastUsed());
  setValue(key::site::lastVariant, site.lastVariant());

  // password
  beginGroup(key::site::password);
  setValue(key::site::contentType, site.password()->contentType());
  switch (site.password()->contentType())
  {
    case Content::None:
    {
      break;
    }
    case Content::Generated:
    {
      GeneratedPassword *pw = (GeneratedPassword*) site.password();
      setValue(key::site::type, pw->type());
      setValue(key::site::counter, pw->counter());
      break;
    }
    case Content::Stored:
    {
      StoredContent *pw = (StoredContent*) site.password();
      setValue(key::site::content, pw->content());
      break;
    }
  }
  endGroup();

  // login
  beginGroup(key::site::login);
  setValue(key::site::contentType, site.login()->contentType());
  switch (site.login()->contentType())
  {
    case Content::None:
    {
      break;
    }
    case Content::Generated:
    {
      GeneratedLogin *login = (GeneratedLogin*) site.login();
      setValue(key::site::counter, login->counter());
      break;
    }
    case Content::Stored:
    {
      StoredContent *login = (StoredContent*) site.login();
      setValue(key::site::content, login->content());
      break;
    }
  }
  endGroup();

  // answer
  beginGroup(key::site::answer);
  setValue(key::site::contentType, site.answer()->contentType());
  switch (site.answer()->contentType())
  {
    case Content::None:
    {
      break;
    }
    case Content::Generated:
    {
      GeneratedAnswer *answer = (GeneratedAnswer*) site.answer();
      setValue(key::site::context, answer->context());
      setValue(key::site::counter, answer->counter());
      break;
    }
    case Content::Stored:
    {
      StoredContent *answer = (StoredContent*) site.answer();
      setValue(key::site::content, answer->content());
      break;
    }
  }
  endGroup();
}

Site Settings::loadSite()
{
  Content *password;
  Content *login;
  Content *answer;

  // password
  beginGroup(key::site::password);
  switch (value<int>(key::site::contentType))
  {
    case Content::None:
      password = new Content();
      break;
    case Content::Generated:
      password = new GeneratedPassword(value<MPSiteType>(key::site::type), value<uint32_t>(key::site::counter));
      break;
    case Content::Stored:
      password = new StoredContent(value<QString>(key::site::content));
      break;
  }
  endGroup();

  // login
  beginGroup(key::site::login);
  switch (value<int>(key::site::contentType))
  {
    case Content::None:
      login = new Content();
      break;
    case Content::Generated:
      login = new GeneratedLogin(value<uint32_t>(key::site::counter));
      break;
    case Content::Stored:
      login = new StoredContent(value<QString>(key::site::content));
      break;
  }
  endGroup();

  // answer
  beginGroup(key::site::answer);
  switch (value<int>(key::site::contentType))
  {
    case Content::None:
      answer = new Content();
      break;
    case Content::Generated:
      answer = new GeneratedAnswer(value<QString>(key::site::context), value<uint32_t>(key::site::counter));
      break;
    case Content::Stored:
      answer = new StoredContent(value<QString>(key::site::content));
      break;
  }
  endGroup();

  return Site(
    value<QString>(key::site::name),
    password,
    login,
    answer,
    value<QString>(key::site::category),
    value<QString>(key::site::url),
    value<uint>(key::site::lastUsed),
    value<MPSiteVariant>(key::site::lastVariant)
  );
}

void Settings::removeSite()
{
  remove(key::site::name);
  remove(key::site::category);
  remove(key::site::url);
  remove(key::site::lastUsed);
  remove(key::site::lastVariant);
  remove(key::site::password);
  remove(key::site::login);
  remove(key::site::answer);
}
