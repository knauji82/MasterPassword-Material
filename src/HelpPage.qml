/**
 * HelpPage.qml
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

Page {
    id: page
    title: qsTr("Help")

    tabs: [
        {
            text: qsTr("Help"),
            icon: "action/help"
        },
        {
            text: qsTr("About"),
            icon: "action/info"
        }
    ]

    TabView {
        id: tabView
        anchors.fill: parent
        currentIndex: page.selectedTab
        model: tabs
    }

    VisualItemModel {
        id: tabs

        // Tab 1 "Help"
        Rectangle {
            width: tabView.width
            height: tabView.height

            View {
                width: units.dp(550)
                height: units.dp(150)
                anchors.centerIn: parent
                elevation: 1

                Label {
                    anchors.centerIn: parent
                    text: qsTr("Visit %1 for more information").arg("<a href=\"http://masterpasswordapp.com/\">masterpasswordapp.com</a>")
                    style: "subheading"
                    onLinkActivated: Qt.openUrlExternally("http://masterpasswordapp.com/")
                }
            }
        }

        // Tab 2 "About"
        Rectangle {
            width: tabView.width
            height: tabView.height

            View {
                width: units.dp(550)
                height: units.dp(350)
                anchors.centerIn: parent
                elevation: 1

                ColumnLayout {
                    anchors {
                        fill: parent
                        margins: 10
                    }
                    spacing: 10

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        horizontalAlignment: Text.AlignHCenter
                        text: "MasterPassword-Material"
                        style: "title"
                    }

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        horizontalAlignment: Text.AlignHCenter
                        text: AppVersion
                        style: "title"
                    }

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        horizontalAlignment: Text.AlignHCenter
                        text: "Based on Qt "+QtVersion
                        style: "subheading"
                    }

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        horizontalAlignment: Text.AlignHCenter
                        text: "Buid on " + BuildDate + " " + BuildTime
                        style: "subheading"
                    }

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        horizontalAlignment: Text.AlignHCenter
                        text: "Copyright (c) 2015, Kilian Schweppe"
                        style: "subheading"
                    }

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }
                        horizontalAlignment: Text.AlignHCenter
                        text: "This program is free software; you can redistribute itand/or modify it\nunder the terms of the GNU General Public License version 3 or later"
                        font.pointSize: 10
                        style: "body1"
                    }
                }
            }
        }
    }
}
