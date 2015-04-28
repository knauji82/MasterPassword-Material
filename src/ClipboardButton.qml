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
import QtQuick.Controls 1.2 as Controls
import QtQuick.Controls.Styles 1.2 as ControlStyles
import Material 0.1

Controls.Button {
    height: units.dp(40)

    signal copied()

    style: ControlStyles.ButtonStyle {
        background: View {
            radius: units.dp(2)

            backgroundColor: "transparent"
            tintColor: mouseArea.currentCircle || control.focus || control.hovered
                     ? Qt.rgba(0, 0, 0, mouseArea.currentCircle
                        ? 0.1 : 0.03)
                     : "transparent"
            Ink {
                id: mouseArea
                anchors.fill: parent
                focus: control.focus
                focusWidth: parent.width - units.dp(30)
                focusColor: Qt.darker("white", 1.05)

                Connections {
                    target: control.__behavior
                    onPressed: mouseArea.onPressed(mouse)
                    onCanceled: mouseArea.onCanceled()
                    onReleased: mouseArea.onReleased(mouse)
                }
            }
        }
        label: Item {
            implicitHeight: Math.max(units.dp(36), label.height + units.dp(16))
            implicitWidth: Math.max(units.dp(88), label.width + units.dp(32))

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

