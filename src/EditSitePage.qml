/**
 * EditSitePage.qml
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
import Material.ListItems 0.1 as ListItem
import MasterPassword 1.0

Page {
    title: newSite ? qsTr("New site") : site.siteName

    property bool newSite: true
    property int siteIndex: -1
    readonly property var site: siteIndex >= 0 ? SiteProxyModel.getMap(siteIndex) : {}

    onNewSiteChanged: {
        if (newSite)
            siteIndex = -1
    }

    onSiteChanged: {
        newSite = siteIndex < 0
        type.selectedIndex = type.typeNames.indexOf(newSite ? MPW.typeName(Settings.defaultSiteType()) : MPW.typeName(site.siteType))
    }

    Dialog {
        id: errorDialog
        title: qsTr("Error")
        text: qsTr("%1 already exists.").arg(name.text)
    }

    View {
        anchors.centerIn: parent

        width: units.dp(500)
        height: column.implicitHeight + units.dp(32)

        elevation: 1

        ColumnLayout {
            id: column

            anchors {
                fill: parent
                margins: units.dp(16)
            }

            GridLayout {
                columns: 2
                rowSpacing: 10
                columnSpacing: 10
                anchors.horizontalCenter: parent.horizontalCenter

                Label {
                    text: qsTr("Name")
                    style: "subheading"
                }
                TextField {
                    id: name
                    text: newSite ? "" : site.siteName
                    Layout.fillWidth: true
                    focus: true
                }

                Label {
                    text: qsTr("Type")
                    style: "subheading"
                }
                MenuField {
                    readonly property var typeNames: MPW.typeNames()
                    id: type
                    model: typeNames
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("Counter")
                    style: "subheading"
                }
                TextField {
                    id: counter
                    text: newSite ? 1 : site.siteCounter
                    input.inputMask: "000"
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("Context")
                    style: "subheading"
                }
                TextField {
                    id: context
                    text: newSite ? "" : site.siteContext
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("Categories")
                    style: "subheading"
                }
                TextField {
                    id: categories
                    text: newSite ? "" : site.siteCategories
                    Layout.fillWidth: true
                }
            }

            View {
                anchors {
                    left: parent.left
                    right: parent.right
                    margins: units.dp(16)
                }
                height: units.dp(36)

                elevation: 1
                backgroundColor: Theme.primaryDarkColor

                Label {
                    anchors.centerIn: parent
                    style: "title"
                    text: {
                        if (name.text != "")
                            Backend.passwordForSite(name.text, MPW.typeWithNameAsInt(type.selectedText), parseInt(counter.text), MPW.variantNamePassword(), context.text)
                        else
                            qsTr("Preview")
                    }
                    color: "white"
                }
            }

            RowLayout {
                Layout.alignment: Qt.AlignRight
                spacing: units.dp(8)

                anchors {
                    right: parent.right
                    margins: units.dp(16)
                }

                Button {
                    text: qsTr("Cancel")
                    textColor: Theme.primaryColor
                    onClicked: pageStack.pop()
                }

                Button {
                    text: qsTr("Delete")
                    textColor: Theme.primaryColor
                    onClicked: {
                        SiteProxyModel.remove(siteIndex)
                        Backend.save()
                        pageStack.pop()
                    }
                    visible: !newSite
                }

                Button {
                    text: qsTr("Save")
                    textColor: Theme.primaryColor
                    onClicked: {
                        var err = (newSite) ? SiteProxyModel.insert(name.text, type.selectedText, parseInt(counter.text), context.text, categories.text)
                                            : SiteProxyModel.modify(siteIndex, name.text, type.selectedText, parseInt(counter.text), context.text, categories.text)
                        if (err != -1) {
                            Backend.save()
                            pageStack.pop()
                        }
                        else {
                            errorDialog.open()
                        }
                    }
                }
            }
        }
    }
}


