/**
 * exporter.h
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

#ifndef EXPORTER_H
#define EXPORTER_H

#include "mpw.h"
#include "site.h"
#include "settings.h"

#include <QJsonDocument>

class Exporter
{
public:
  static void toJson(QList<Site> const &sites, MPAlgorithmVersion algorithm_version, QFile &file);
};

#endif // EXPORTER_H
