/**
 * ClipboardButton.qml
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
import QtQuick.Controls.Styles.Material 0.1

Button {
    height: Units.dp(40)

    signal copied()

    style: ButtonStyle {
        label: Item {
            implicitHeight: Math.max(Units.dp(36), label.height + Units.dp(16))
            implicitWidth: Math.max(Units.dp(88), label.width + Units.dp(32))

            Label {
                id: label
                anchors.centerIn: parent
                text: HidePasswords ? Backend.obscure(control.text) : control.text
                style: "title"
                color: Theme.accentColor
            }

        }
    }

    onClicked: {
        Backend.copyToClipboard(text)
        copied()
    }
}

