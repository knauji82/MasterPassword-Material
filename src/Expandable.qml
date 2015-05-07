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

View {
    default property alias expandableContent: column.data

    property alias text: header.text

    property bool expanded: false

    anchors {
        left: parent.left
        right: parent.right
    }

    height: expanded ? header.height + column.implicitHeight + Units.dp(16) : header.height

    function collapse() {
        expanded = false
    }

    function expand() {
        expanded = true
    }

    Behavior on height {
        NumberAnimation { duration: 250 }
    }

    ListItem.Subtitled {
        id: header
        subText: expanded ? qsTr("Click to collapse") : qsTr("Click to expand")
        onClicked: expanded ? collapse() : expand()
    }

    Column {
        id: column
        spacing: Units.dp(10)
        opacity: expanded ? 1 : 0

        Behavior on opacity {
            NumberAnimation { duration: 250 }
        }

        anchors {
            top: header.bottom
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            topMargin: Units.dp(8)
            bottomMargin: Units.dp(8)
            leftMargin: Units.dp(16)
            rightMargin: Units.dp(16)
        }
    }
}
