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
            onTriggered: pageStack.push(Qt.resolvedUrl("EditSitePage.qml"))
        },
        Action {
            name: qsTr("Import/Export")
            iconName: "communication/import_export"
            onTriggered: pageStack.push(Qt.resolvedUrl("ImportExportPage.qml"))
        },
        Action {
            name: qsTr("About")
            iconName: "action/info"
            onTriggered: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
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

        positiveButtonText: qsTr("Ok")
        negativeButtonText: qsTr("Cancel")

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
                        valueText: siteLastVariant
                        subText: Qt.formatDateTime(siteLastUsed, "ddd MMMM d yy hh:mm:ss")
                        onClicked: {
                            var sourceIndex = SiteHistoryModel.mapIndexToSource(index)
                            var proxyIndex = SiteProxyModel.mapIndexFromSource(sourceIndex)
                            if (proxyIndex === -1) {
                                filter.selectedIndex = 0
                                proxyIndex = SiteProxyModel.mapIndexFromSource(sourceIndex)
                            }
                            Backend.copyToClipboard(Backend.passwordForSite(siteName, sitePassword.type, sitePassword.counter, siteLastVariant))
                            siteView.clipboardIndex = proxyIndex
                            snackbar.open(qsTr("Copied password for %1 seconds").arg(ClipboardDuration))
                            SiteProxyModel.updateDate(proxyIndex, siteLastVariant)
                        }
                    }
                }
            }
        }
    }

    DynamicGrid {
        id: siteView
        focus: true

        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: sideBar.left
        }

        model: SiteProxyModel

        spacing: units.dp(12)
        columnWidth: units.dp(300)

        property int clipboardIndex: -1
        readonly property int clipboardSourceIndex: clipboardIndex >= 0 ? SiteProxyModel.mapIndexToSource(clipboardIndex) : -1

        property double clipboardProgress: 0

        /*
        Keys.onReturnPressed: {
            itemAt(selectedIndex).focus = true
        }
        */

        Connections {
            target: SiteProxyModel
            onSortOrderChanged: siteView.update()
            onDataChanged: siteView.update()
        }

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

        delegate: Card {
            height: column.implicitHeight

            elevation: index == siteView.selectedIndex ? 2 : 1

            readonly property bool inClipboard: index === SiteProxyModel.mapIndexFromSource(siteView.clipboardSourceIndex)

            /*
            Keys.onUpPressed: {}
            Keys.onDownPressed: {}
            Keys.onEscapePressed: siteView.focus = true
            */

            function getPassword() {
                switch (sitePassword.contentType) {
                    case ContentType.None:
                        return ""
                    case ContentType.Generated:
                        return Backend.passwordForSite(siteName, sitePassword.type, sitePassword.counter, MPW.variantNamePassword())
                    case ContentType.Stored:
                        return Backend.decrypt(sitePassword.content)
                }
            }

            function getLogin() {
                switch (siteLogin.contentType) {
                    case ContentType.None:
                        return ""
                    case ContentType.Generated:
                        return Backend.passwordForSite(siteName, MPW.typeWithNameAsInt(MPW.typeNameName()), siteLogin.counter, MPW.variantNameLogin())
                    case ContentType.Stored:
                        return Backend.decrypt(siteLogin.content)
                }
            }

            function getAnswer() {
                switch (siteAnswer.contentType) {
                    case ContentType.None:
                        return ""
                    case ContentType.Generated:
                        return Backend.passwordForSite(siteName, MPW.typeWithNameAsInt(MPW.typeNamePhrase()), siteAnswer.counter, MPW.variantNameAnswer())
                    case ContentType.Stored:
                        return Backend.decrypt(siteAnswer.content)
                }
            }

            MouseArea {
                anchors.fill: parent
                onReleased: onSiteSelected(index)
            }

            ColumnLayout {
                id: column
                anchors.fill: parent

                Label {
                    anchors {
                        left: parent.left
                        right: parent.right
                    }
                    text: siteName
                    style: "headline"
                    horizontalAlignment: Text.AlignHCenter
                }

                ClipboardButton {
                    text: getPassword()
                    onCopied: {
                        siteView.clipboardIndex = index
                        snackbar.open(qsTr("Copied password for %1 seconds").arg(ClipboardDuration))
                        SiteProxyModel.updateDate(index, MPW.variantNamePassword())
                    }

                    anchors {
                        left: parent.left
                        right: parent.right
                        margins: units.dp(8)
                    }

                    visible: text != ""

                    Label {
                        text: {
                            var t = qsTr("Password")
                            if (sitePassword.isGenerated)
                                t += ": " + MPW.typeName(sitePassword.type)
                            return t
                        }
                        anchors {
                            top: parent.top
                            horizontalCenter: parent.horizontalCenter
                        }
                    }
                }

                ClipboardButton {
                    text: getLogin()
                    onCopied: {
                        siteView.clipboardIndex = index
                        snackbar.open(qsTr("Copied login for %1 seconds").arg(ClipboardDuration))
                        SiteProxyModel.updateDate(index, MPW.variantNameLogin())
                    }

                    anchors {
                        left: parent.left
                        right: parent.right
                        margins: units.dp(8)
                    }

                    visible: text != ""

                    Label {
                        text: "Login"
                        anchors {
                            top: parent.top
                            horizontalCenter: parent.horizontalCenter
                        }
                    }
                }

                ClipboardButton {
                    text: getAnswer()
                    onCopied: {
                        siteView.clipboardIndex = index
                        snackbar.open(qsTr("Copied answer for %1 seconds").arg(ClipboardDuration))
                        SiteProxyModel.updateDate(index, MPW.variantNameAnswer())
                    }

                    anchors {
                        left: parent.left
                        right: parent.right
                        margins: units.dp(8)
                    }

                    visible: text != ""

                    Label {
                        text: "answer"
                        anchors {
                            top: parent.top
                            horizontalCenter: parent.horizontalCenter
                        }
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


