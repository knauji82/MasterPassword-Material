/**
 * content.cc
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

#include "content.h"

Content * passwordFromMap(QVariantMap const &map)
{
  switch (map["contentType"].toUInt())
  {
    case Content::None:
      return new Content();
    case Content::Generated:
      return new GeneratedPassword(map["type"].toUInt(), map["counter"].toUInt());
    case Content::Stored:
      return new StoredContent(map["content"].toString());
  }
}


Content * loginFromMap(QVariantMap const &map)
{
  switch (map["contentType"].toUInt())
  {
    case Content::None:
      return new Content();
    case Content::Generated:
      return new GeneratedLogin(map["counter"].toUInt());
    case Content::Stored:
      return new StoredContent(map["content"].toString());
  }
}


Content * answerFromMap(QVariantMap const &map)
{
  switch (map["contentType"].toUInt())
  {
    case Content::None:
      return new Content();
    case Content::Generated:
      return new GeneratedAnswer(map["context"].toString(), map["counter"].toUInt());
    case Content::Stored:
      return new StoredContent(map["content"].toString());
  }
}
