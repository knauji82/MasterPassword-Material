/**
 * ViewSwitcher.qml
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

import QtQuick 2.3

Item {
    property int index: 0

    implicitWidth: isValidIndex(index) ? children[index].implicitWidth : 0
    implicitHeight: isValidIndex(index) ? children[index].implicitHeight : 0

    onIndexChanged: {
        for (var i=0; i < children.length; i++)
            children[i].visible = i == index
    }

    onChildrenChanged: children[children.length-1].visible = children.length-1 == index

    function isValidIndex(i) {
        return i >= 0 && i < children.length
    }
}

