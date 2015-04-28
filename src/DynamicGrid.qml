/**
 * DynamicGrid.qml
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

Flickable {
    property alias model: view.model
    property alias delegate: view.delegate
    property alias count: view.count

    property int columnWidth: units.dp(100)
    property int spacing: 0

    property int selectedIndex: -1

    contentHeight: view.height

    function update() {
        var m = view.model
        view.model = []
        view.model = m
    }

    function itemAt(index) {
        return view.itemAt(index)
    }

    Keys.onLeftPressed: {
        selectedIndex--
        if (selectedIndex == -1)
            selectedIndex = count - 1
    }
    Keys.onRightPressed: {
        selectedIndex++
        if (selectedIndex == count)
            selectedIndex = 0
    }
    Keys.onUpPressed: {
        selectedIndex -= view.columns
        if (selectedIndex < 0)
            selectedIndex += count
    }
    Keys.onDownPressed: {
        selectedIndex += view.columns
        if (selectedIndex >= count)
            selectedIndex -= count
    }
    Keys.onEscapePressed: {
        selectedIndex = -1
    }

    Repeater {
        id: view

        anchors {
            left: parent.left
            right: parent.right
            top: parent.top
        }

        readonly property int columns: parent.width / (columnWidth + spacing)
        property var columnHeights
        property int nextItem

        function getColumn(height) {
            var c = 0
            for (var i=c+1; i < columnHeights.length; i++)
                if (columnHeights[i] < columnHeights[c] && (columnHeights[i] === 0 || columnHeights[c] - columnHeights[i] > height/2))
                    c = i
            return c
        }

        function alignItems(reset) {
            if (reset) {
                columnHeights = []
                for (var i=0; i < columns; i++)
                    columnHeights[i] = 0
                nextItem = 0
            }
            for (i=nextItem; i < count; i++) {
                var item = itemAt(i)
                if (!item || !item.parent)
                    continue
                var column = getColumn(item.height)
                item.width = columnWidth
                item.anchors.left = parent.left
                item.anchors.leftMargin = columnWidth * (column % columns) + spacing * (column % columns + 1)
                item.anchors.top = parent.top
                item.anchors.topMargin = columnHeights[column] + spacing
                columnHeights[column] += item.height + spacing
                nextItem = i + 1
            }
            height = Math.max.apply(null, columnHeights) + spacing
        }

        onModelChanged: alignItems(true)
        onColumnsChanged: alignItems(true)
        onItemAdded: alignItems(index < nextItem)
        onItemRemoved: alignItems(true)
    }
}
