/**
 * qmlapplicationengine.cc
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

#include "qmlapplicationengine.h"

QObject * QmlApplicationEngine::load(QUrl const &url)
{
  component_ = new QQmlComponent(this, this);
  component_->loadUrl(url);

  if (!component_->isReady())
  {
    qWarning("%s", qPrintable(component_->errorString()));
    return nullptr;
  }
  root_ = component_->create();
  return root_;
}

