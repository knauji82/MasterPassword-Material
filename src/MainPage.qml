/**
 * MainPage.qml
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
import QtQuick.Controls 1.2 as Controls
import QtQuick.Controls.Styles 1.2 as ControlStyles
import Material 0.1
import Material.ListItems 0.1 as ListItem
import MasterPassword 1.0

Page {
    Connections {
        target: Backend
        onRequestLogout: {
            pageStack.pop()
            Backend.logout()
        }
    }

    function onSiteSelected(index) {
        pageStack.push(Qt.resolvedUrl("EditSitePage.qml"))
        pageStack.currentItem.siteIndex = index
    }

    actionBar.maxActionCount: 5

    actions: [
        Action {
            name: qsTr("Add")
            iconName: "content/add"
            onTriggered: {
                pageStack.push(Qt.resolvedUrl("EditSitePage.qml"))
            }
        },
        Action {
            name: qsTr("Help")
            iconName: "action/help"
            onTriggered: pageStack.push(Qt.resolvedUrl("HelpPage.qml"))
        },
        Action {
            name: qsTr("Import/Export")
            iconName: "communication/import_export"
            onTriggered: pageStack.push(Qt.resolvedUrl("ImportExportPage.qml"))

        },
        Action {
            name: qsTr("Settings")
            iconName: "action/settings"
            onTriggered: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
        },
        Action {
            name: qsTr("Quit")
            iconName: "action/exit_to_app"
            onTriggered: Qt.quit()
        }

    ]

    backAction: Action {
        name: qsTr("Back")
        iconName: "navigation/arrow_back"
        onTriggered: confirmLogoutDialog.show()
    }

    Dialog {
        id: confirmLogoutDialog
        title: qsTr("Logout now?")

        onAccepted: {
            Backend.logout()
            pageStack.pop()
        }
    }

    Snackbar {
        id: snackbar
        z: 1
    }

    Sidebar {
        id: sideBar
        mode: "right"

        Column {
            width: parent.width

            Expandable {
                text: qsTr("Options")

                Label {
                    text: qsTr("Variant") + ":"
                    style: "subheading"
                }

                MenuField {
                    id: variant
                    width: parent.width
                    model: MPW.variantNames()
                    helperText: qsTr("The kind of content to generate") + "\n- " +
                                qsTr("The Password to log in with") + "\n- " +
                                qsTr("The username to log in as") + "\n- " +
                                qsTr("The answer to a security question")
                }

                Item {
                    /* Spacer */
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    height: units.dp(10)
                }

                Label {
                    text: qsTr("Order") + ":"
                    style: "subheading"
                }

                MenuField {
                    id: order
                    width: parent.width
                    model: [qsTr("Ascending"), qsTr("Descending")]
                    helperText: qsTr("Sort by name")
                    selectedIndex: Settings.sortOrder()
                    onSelectedIndexChanged: {
                        SiteProxyModel.sort(selectedIndex)
                        Settings.setSortOrder(selectedIndex)
                    }
                }

                Item {
                    /* Spacer */
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    height: units.dp(10)
                }

                Label {
                    text: qsTr("Filter") + ":"
                    style: "subheading"
                }

                MenuField {
                    id: filter
                    width: parent.width
                    model: CategoryModel
                    helperText: qsTr("Filter sites by a category")
                    onSelectedTextChanged: SiteProxyModel.categoryFilter = selectedIndex == 0 ? "" : selectedText

                    Connections {
                        target: CategoryModel
                        onModelChanged: filter.selectedIndex = 0
                    }
                }
            }

            Expandable {
                text: qsTr("Recently used")
                expanded: true

                Repeater {
                    model: SiteHistoryModel
                    delegate: ListItem.Subtitled {
                        text: siteName
                        subText: Qt.formatDateTime(siteLastUsed, "ddd MMMM d yy hh:mm:ss")
                        onClicked: {
                            var sourceIndex = SiteHistoryModel.mapIndexToSource(index)
                            var proxyIndex = SiteProxyModel.mapIndexFromSource(sourceIndex)
                            if (proxyIndex == -1) {
                                filter.selectedIndex = 0
                                proxyIndex = SiteProxyModel.mapIndexFromSource(sourceIndex)
                            }
                            Backend.copyToClipboard(Backend.passwordForSite(siteName, siteType, siteCounter, variant.selectedText, siteContext))
                            siteView.clipboardIndex = proxyIndex
                            snackbar.open(qsTr("Copied password for %1 seconds").arg(ClipboardDuration))
                            SiteProxyModel.updateDate(proxyIndex)
                        }
                    }
                }
            }
        }
    }

    GridView {
        id: siteView

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: sideBar.left
        }

        cellWidth: units.dp(300)
        cellHeight: units.dp(200)

        model: SiteProxyModel

        property int clipboardIndex: -1
        readonly property int clipboardSourceIndex: clipboardIndex >= 0 ? SiteProxyModel.mapIndexToSource(clipboardIndex) : -1

        property double clipboardProgress: 0

        Connections {
            target: ResetClipboardThread
            onProgressChanged: {
                siteView.clipboardProgress = progress
            }
            onFinished: {
                siteView.clipboardProgress = 0
                siteView.clipboardIndex = -1
            }
        }

        delegate: Item {
            width: units.dp(300)
            height: units.dp(200)

            readonly property bool inClipboard: index === SiteProxyModel.mapIndexFromSource(siteView.clipboardSourceIndex)

            Card {
                anchors {
                    fill: parent
                    margins: units.dp(16)
                }

                MouseArea {
                    anchors.fill: parent
                    onReleased: {
                        onSiteSelected(index)
                    }
                }

                Icon {
                    id: caller
                    width: units.dp(25)
                    height: units.dp(25)
                    anchors.margins: units.dp(5)
                    name: "navigation/more_vert"

                    anchors {
                        top: parent.top
                        right: parent.right
                    }

                    Ink {
                        anchors.fill: parent
                        onClicked: menu.open(caller, 0, units.dp(30))
                    }

                    Dropdown {
                        id: menu
                        width: units.dp(150)
                        implicitHeight: column.height
                        anchor: Item.TopRight

                        Column {
                            id: column
                            width: parent.width

                            ListItem.Standard {
                                text: qsTr("Delete")
                                onClicked: {
                                    SiteProxyModel.remove(index)
                                    menu.close()
                                }
                            }

                            ListItem.Standard {
                                text: qsTr("Increase counter")
                                onClicked: {
                                    SiteProxyModel.modify(index, siteName, MPW.typeName(siteType), siteCounter+1, siteContext, siteCategory)
                                    menu.close()
                                }
                            }
                        }
                    }
                }

                ColumnLayout {
                    anchors.fill: parent

                    Label {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        text: siteName
                        font.pixelSize: units.dp(32)
                        horizontalAlignment: Text.AlignHCenter
                    }

                    GridLayout {
                        columns: 2
                        anchors.horizontalCenter: parent.horizontalCenter

                        Label {
                            text: qsTr("Type")
                        }
                        Label {
                            text: MPW.typeName(siteType)
                        }

                        Label {
                            text: qsTr("Counter")
                        }
                        Label {
                            text: siteCounter
                        }
                    }

                    Controls.Button {
                        anchors {
                            left: parent.left
                            right: parent.right
                            bottom: parent.bottom
                            margins: 10
                        }
                        height: units.dp(40)

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
                                    text: control.text
                                    style: "title"
                                    color: Theme.accentColor
                                }

                            }
                        }

                        text: if (variant.selectedText) {
                            var pw = Backend.passwordForSite(siteName, siteType, siteCounter, variant.selectedText, siteContext)
                            HidePasswords ? Backend.obscure(pw) : pw
                        }

                        onClicked: {
                            Backend.copyToClipboard(Backend.passwordForSite(siteName, siteType, siteCounter, variant.selectedText, siteContext))
                            siteView.clipboardIndex = index
                            snackbar.open(qsTr("Copied password for %1 seconds").arg(ClipboardDuration))
                            SiteProxyModel.updateDate(index)
                        }
                    }
                    ProgressBar {
                        opacity: inClipboard ? 1 : 0
                        color: theme.accentColor
                        Layout.fillWidth: true

                        NumberAnimation on value {
                            running: inClipboard
                            from : siteView.clipboardProgress
                            to: 1
                            duration: (ClipboardDuration * 1000) * (1 - siteView.clipboardProgress)
                        }
                    }
                }
            }
        }
    }
}


