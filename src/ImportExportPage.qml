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
import Material 0.1

Page {
    id: page
    title: qsTr("Import/Export")

    tabs: [
        qsTr("Import"),
        qsTr("Export")
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
                width: Units.dp(550)
                height: Units.dp(300)
                anchors.centerIn: parent
                elevation: 1

                Column {
                    anchors {
                        centerIn: parent
                    }

                    Row {
                        TextField {
                            id: importFile
                            text: fileDialog.filePath
                            width: Units.dp(350)
                            placeholderText: qsTr("Choose a file to import")
                        }
                        Button {
                            text: qsTr("Choose")
                            onClicked: fileDialog.open()
                        }
                    }

                    CheckBox {
                        id: overwrite
                        anchors.horizontalCenter: parent.horizontalCenter
                        text: qsTr("Replace existing sites")
                    }
                }

                Button {
                    anchors {
                        bottom: parent.bottom
                        horizontalCenter: parent.horizontalCenter
                        margins: Units.dp(8)
                    }
                    enabled: importFile.text.length > 0 && Backend.existsFile(importFile.text)
                    width: Units.dp(300)
                    elevation: 1
                    text: qsTr("Import")
                    onClicked: {
                        Backend.importFile(importFile.text, overwrite.checked)
                        pageStack.pop()
                    }
                }

                FileDialog {
                    id: fileDialog
                    title: qsTr("Import a file")
                    nameFilters: ["Json (*.json)"]
                }
            }
        }

        // Tab 2 "Export"
        Rectangle {
            width: tabView.width
            height: tabView.height

            View {
                width: Units.dp(550)
                height: Units.dp(300)
                anchors.centerIn: parent
                elevation: 1

                Column {
                    anchors {
                        centerIn: parent
                    }

                    Row {
                        TextField {
                            id: exportDir
                            text: dirDialog.folderPath
                            width: Units.dp(350)
                            placeholderText: qsTr("Destination directory")
                        }
                        Button {
                            text: qsTr("Choose")
                            onClicked: dirDialog.open()
                        }
                    }

                    GridLayout {
                        columns: 2

                        Label {
                            text: qsTr("File name")
                            style: "subheading"
                        }
                        TextField {
                            id: fileName
                            text: new Date().toLocaleString(Qt.locale(), "yyyy-MM-dd_HH-mm")+"_export"
                            placeholderText: qsTr("Destination file")
                        }

                        Label {
                            text: qsTr("Format")
                            style: "subheading"
                        }
                        MenuField {
                            id: format
                            Layout.fillWidth: true
                            model: ["Json"]
                        }
                    }

                    ViewSwitcher {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        index: format.selectedIndex

                        CheckBox {
                            id: compatibility
                            text: qsTr("Compatibility mode (for Android)")
                            anchors.horizontalCenter: parent.horizontalCenter
                        }
                    }
                }

                Button {
                    anchors {
                        bottom: parent.bottom
                        horizontalCenter: parent.horizontalCenter
                        margins: Units.dp(8)
                    }
                    enabled: exportDir.text.length > 0 && fileName.text.length > 0 && Backend.existsDir(exportDir.text)
                    width: Units.dp(300)
                    elevation: 1
                    text: qsTr("Export")
                    onClicked: {
                        Backend.exportFile(exportDir.text+"/"+fileName.text+"."+format.selectedText.toLowerCase(), {"compatibility": compatibility.checked})
                        pageStack.pop()
                    }
                }

                FileDialog {
                    id: dirDialog
                    title: qsTr("Destination directory")
                    folder: "file://" + (Qt.platform.os == "windows" ? "/" : "") + Backend.exportDirectory()
                    selectFolder: true
                }
            }
        }
    }
}
