/**
 * Expandable.qml
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
import Material 0.1
import Material.ListItems 0.1 as ListItem

Column {
    default property alias expandableContent: column.data

    property alias text: header.text
    property alias expanded: header.expanded
    property int contentMargin: 0

    height: header.expanded ? header.height + column.implicitHeight + Units.dp(8) : header.height

    anchors {
        left: parent.left
        right: parent.right
    }

    Behavior on height {
        NumberAnimation { duration: 250 }
    }

    ListItem.SectionHeader {
        id: header
    }

    Column {
        id: column
        opacity: header.expanded ? 1 : 0

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: contentMargin
            rightMargin: contentMargin
        }

        Behavior on opacity {
            NumberAnimation { duration: 250 }
        }
    }
}
