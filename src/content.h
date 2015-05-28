/**
 * content.h
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

#ifndef CONTENT
#define CONTENT

#include <QObject>
#include <QtQml>

#include "mpw.h"

class Content : public QObject
{
  Q_OBJECT
  Q_ENUMS(Type)

public:
  enum Type
  {
    None,
    Generated,
    Stored
  };

  static inline void registerQml()
  {
    qmlRegisterType<Content>("MasterPassword", 1, 0, "ContentType");
  }

  virtual inline Type contentType() const { return None; }

  inline bool isGenerated() const { return contentType() == Generated; }

  inline bool isStored() const { return contentType() == Stored; }

  virtual QVariantMap toMap() const;
};

class StoredContent : public Content
{
public:
  StoredContent(QString const &content) : content_(content) {}

  Type contentType() const { return Stored; }

  inline QString const & content() const { return content_; }

  inline void setContent(QString const &content) { content_ = content; }

  QVariantMap toMap() const;

private:
  QString content_;
};

class GeneratedPassword : public Content
{
public:
  GeneratedPassword(MPSiteType type, uint32_t counter)
      : type_(type), counter_(counter) {}

  inline Type contentType() const { return Generated; }

  inline MPSiteType type() const { return type_; }

  inline void setType(MPSiteType type) { type_ = type; }

  inline uint32_t counter() const { return counter_; }

  inline void setCounter(uint32_t counter) { counter_ = counter; }

  QVariantMap toMap() const;

private:
  MPSiteType type_;
  uint32_t counter_;
};

class GeneratedLogin : public Content
{
public:
  GeneratedLogin(uint32_t counter) : counter_(counter) {}

  inline Type contentType() const { return Generated; }

  inline MPSiteType type() const { return MPSiteTypeGeneratedName; }

  inline uint32_t counter() const { return counter_; }

  inline void setCounter(uint32_t counter) { counter_ = counter; }

  QVariantMap toMap() const;

private:
  uint32_t counter_;
};

class GeneratedAnswer : public Content
{
public:
  GeneratedAnswer(QString const &context, uint32_t counter)
      : context_(context), counter_(counter) {}

  inline Type contentType() const { return Generated; }

  inline MPSiteType type() const { return MPSiteTypeGeneratedPhrase; }

  inline QString const & context() const { return context_; }

  inline void setContext(QString const &context) { context_ = context; }

  inline uint32_t counter() const { return counter_; }

  inline void setCounter(uint32_t counter) { counter_ = counter; }

  QVariantMap toMap() const;

private:
  QString context_;
  uint32_t counter_;
};

Content * passwordFromMap(QVariantMap const &map);

Content * loginFromMap(QVariantMap const &map);

Content * answerFromMap(QVariantMap const &map);

#endif // CONTENT

