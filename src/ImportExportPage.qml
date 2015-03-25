/**
 * ImportExportPage.qml
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
import QtQuick.Layouts 1.1
import QtQuick.Dialogs 1.0
import Qt.labs.folderlistmodel 2.1
import Material 0.1
import Material.ListItems 0.1 as ListItem

Page {
    id: page
    title: qsTr("Import/Export")

    tabs: [
        qsTr("Import"),
        //qsTr("Export")
    ]

    TabView {
        id: tabView
        anchors.fill: parent
        currentIndex: page.selectedTab
        model: tabs
    }

    VisualItemModel {
        id: tabs

        // Tab 1 "Import"
        Rectangle {
            width: tabView.width
            height: tabView.height

            View {
                width: units.dp(550)
                height: units.dp(300)
                anchors.centerIn: parent
                elevation: 1

                Column {
                    anchors {
                        centerIn: parent
                    }

                    Row {
                        TextField {
                            id: importFile
                            text: fileDialog.fileUrl
                            width: units.dp(350)
                            helperText: qsTr("Compatible with Master Password for Android")
                        }
                        Button {
                            text: qsTr("Choose")
                            onClicked: fileDialog.open()
                        }
                    }

                    Checkbox {
                        id: overwrite
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Replace existing sites")
                    }
                }

                Button {
                    anchors {
                        bottom: parent.bottom
                        horizontalCenter: parent.horizontalCenter
                        margins: units.dp(8)
                    }
                    enabled: importFile.text.length > 0 && Backend.existsFile(importFile.text)
                    width: units.dp(300)
                    elevation: 1
                    text: qsTr("Import")
                    onClicked: {
                        Backend.importFile(importFile.text, overwrite.checked)
                        pageStack.pop()
                    }
                }

                FileDialog {
                    id: fileDialog
                    title: "Import"
                    nameFilters: ["Json (*.json)"]
                }
            }
        }

        // Tab 2 "Export"
        Rectangle {
            width: tabView.width
            height: tabView.height

            View {
                width: units.dp(550)
                height: units.dp(300)
                anchors.centerIn: parent
                elevation: 1

                FileDialog {
                    id: dirDialog
                    title: "Export"
                    selectFolder: true
                    onAccepted: exportDir.text = fileUrl
                }
            }
        }
    }
}