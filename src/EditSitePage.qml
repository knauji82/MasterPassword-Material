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
    title: newSite ? qsTr("New site") : currentSite.siteName

    property bool newSite: true
    property int siteIndex: -1
    readonly property var currentSite: siteIndex >= 0 ? SiteProxyModel.getMap(siteIndex) : {}

    onNewSiteChanged: {
        if (newSite)
            siteIndex = -1
    }

    onCurrentSiteChanged: {
        newSite = siteIndex < 0
        passwordType.selectedIndex = passwordType.typeNames.indexOf(!newSite && currentSite.sitePassword.isGenerated
                                                                    ? MPW.typeName(currentSite.sitePassword.type)
                                                                    : MPW.typeName(Settings.defaultSiteType()))
    }

    actions: [
        Action {
            name: qsTr("Delete")
            iconName: "action/delete"
            enabled: siteIndex >= 0
            onTriggered: {
                SiteProxyModel.remove(siteIndex)
                Backend.save()
                pageStack.pop()
            }
        },
        Action {
            name: qsTr("Save")
            iconName: "content/save"
            enabled: siteName.text != "" && (passwordSelection.selectedIndex > 0 || loginSelection.selectedIndex > 0 || answerSelection.selectedIndex > 0)
            onTriggered: {
                var password = {}
                var login = {}
                var answer = {}

                // password
                switch (passwordSelection.selectedIndex) {
                    case ContentType.Generated:
                        password = {
                            "contentType": ContentType.Generated,
                            "type": MPW.typeWithNameAsInt(passwordType.selectedText),
                            "counter": parseInt(passwordCounter.text)
                        }
                        break
                    case ContentType.Stored:
                        password = {
                            "contentType": ContentType.Stored,
                            "content": Backend.encrypt(passwordContent.text)
                        }
                        break
                }

                // login
                switch (loginSelection.selectedIndex) {
                    case ContentType.Generated:
                        login = {
                            "contentType": ContentType.Generated,
                            "counter": parseInt(loginCounter.text)
                        }
                        break
                    case ContentType.Stored:
                       login = {
                            "contentType": ContentType.Stored,
                            "content": Backend.encrypt(loginContent.text)
                        }
                }

                // answer
                switch (answerSelection.selectedIndex) {
                    case ContentType.Generated:
                        answer = {
                            "contentType": ContentType.Generated,
                            "context": answerContext.text,
                            "counter": parseInt(answerCounter.text)
                        }
                        break
                    case ContentType.Stored:
                        answer = {
                            "contentType": ContentType.Stored,
                            "content": Backend.encrypt(answerContent.text)
                        }
                        break
                }

                var site = {
                    "name": siteName.text,
                    "category": siteCategory.text,
                    "lastUsed": newSite ? 0 : currentSite.siteLastUsed,
                    "lastVariant": newSite? "" : currentSite.lastVariant,
                    "password": password,
                    "login": login,
                    "answer": answer
                }

                var err = (newSite) ? SiteProxyModel.insert(site) : SiteProxyModel.modify(siteIndex, site)
                if (err !== -1) {
                    Backend.save()
                    pageStack.pop()
                }
                else {
                    errorDialog.open()
                }
            }
        }
    ]

    Dialog {
        id: errorDialog
        title: qsTr("Error")
        text: qsTr("%1 already exists.").arg(siteName.text)
    }

    Flickable {
        anchors {
            fill: parent
            margins: units.dp(32)
        }
        contentHeight: content.height
        contentWidth: width

        Column {
            id: content
            anchors.centerIn: parent
            spacing: units.dp(16)

            View {
                width: units.dp(500)
                height: columnSite.implicitHeight + units.dp(32)

                elevation: 1

                GridLayout {
                    id: columnSite

                    anchors {
                        fill: parent
                        margins: units.dp(16)
                    }

                    columns: 2
                    rowSpacing: units.dp(8)
                    columnSpacing: units.dp(8)

                    Label {
                        text: qsTr("Name")
                        style: "subheading"
                    }
                    TextField {
                        id: siteName
                        text: newSite ? "" : currentSite.siteName
                        Layout.fillWidth: true
                        focus: true
                    }

                    Label {
                        text: qsTr("Category")
                        style: "subheading"
                    }
                    TextField {
                        id: siteCategory
                        text: newSite ? "" : currentSite.siteCategory
                        Layout.fillWidth: true
                    }
                }
            }

            View {
                width: units.dp(500)
                height: columnPassword.implicitHeight + units.dp(32)

                elevation: 1

                ColumnLayout {
                    id: columnPassword

                    anchors {
                        fill: parent
                        margins: units.dp(16)
                    }

                    GridLayout {
                        columns: 2
                        rowSpacing: units.dp(8)
                        columnSpacing: units.dp(8)

                        Label {
                            text: qsTr("Password")
                            style: "subheading"
                        }
                        MenuField {
                            id: passwordSelection
                            model: [qsTr("None"), qsTr("Generate"), qsTr("Store")]
                            selectedIndex: !newSite ? currentSite.sitePassword.contentType : 1
                            Layout.fillWidth: true
                        }
                    }

                    ViewSwitcher {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        index: passwordSelection.selectedIndex - 1

                        Column {
                            anchors.fill: parent
                            spacing: units.dp(8)

                            GridLayout {
                                anchors {
                                    left: parent.left
                                    right: parent.right
                                }

                                columns: 2
                                rowSpacing: units.dp(8)
                                columnSpacing: units.dp(8)

                                Label {
                                    text: qsTr("Type")
                                    style: "subheading"
                                }
                                MenuField {
                                    readonly property var typeNames: MPW.typeNames()
                                    id: passwordType
                                    model: typeNames
                                    Layout.fillWidth: true
                                }

                                Label {
                                    text: qsTr("Counter")
                                    style: "subheading"
                                }
                                TextField {
                                    id: passwordCounter
                                    text: !newSite && currentSite.sitePassword.isGenerated ? currentSite.sitePassword.counter : 1
                                    input.inputMask: "000"
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
                                    text: siteName.text != ""
                                          ? Backend.passwordForSite(siteName.text, MPW.typeWithNameAsInt(passwordType.selectedText),
                                                                    parseInt(passwordCounter.text), MPW.variantNamePassword())
                                          : qsTr("Preview")
                                    color: "white"
                                }
                            }
                        }

                        GridLayout {
                            anchors {
                                left: parent.left
                                right: parent.right
                            }

                            columns: 2
                            rowSpacing: units.dp(8)
                            columnSpacing: units.dp(8)

                            Label {
                                text: qsTr("Content")
                                style: "subheading"
                            }
                            TextField {
                                id: passwordContent
                                text: !newSite && currentSite.sitePassword.isStored ? Backend.decrypt(currentSite.sitePassword.content) : ""
                                placeholderText: !newSite && currentSite.sitePassword.isStored && text == "" ? qsTr("Encrypted with a different master key") : ""
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
            }

            View {
                width: units.dp(500)
                height: columnLogin.implicitHeight + units.dp(32)

                elevation: 1

                ColumnLayout {
                    id: columnLogin

                    anchors {
                        fill: parent
                        margins: units.dp(16)
                    }

                    GridLayout {
                        columns: 2
                        rowSpacing: units.dp(8)
                        columnSpacing: units.dp(8)

                        Label {
                            text: qsTr("Login")
                            style: "subheading"
                        }
                        MenuField {
                            id: loginSelection
                            model: [qsTr("None"), qsTr("Generate"), qsTr("Store")]
                            selectedIndex: !newSite ? currentSite.siteLogin.contentType : 0
                            Layout.fillWidth: true
                        }
                    }

                    ViewSwitcher {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        index: loginSelection.selectedIndex -1

                        Column {
                            anchors.fill: parent
                            spacing: units.dp(8)

                            GridLayout {
                                columns: 2
                                rowSpacing: units.dp(8)
                                columnSpacing: units.dp(8)

                                Label {
                                    text: qsTr("Counter")
                                    style: "subheading"
                                }
                                TextField {
                                    id: loginCounter
                                    text: !newSite && currentSite.siteLogin.isGenerated ? currentSite.siteLogin.counter : 1
                                    input.inputMask: "000"
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
                                    text: siteName.text != ""
                                          ? Backend.passwordForSite(siteName.text, MPW.typeWithNameAsInt(MPW.typeNameName()),
                                                                    parseInt(loginCounter.text), MPW.variantNameLogin())
                                          : qsTr("Preview")
                                    color: "white"
                                }
                            }
                        }

                        GridLayout {
                            anchors {
                                left: parent.left
                                right: parent.right
                            }

                            columns: 2
                            rowSpacing: units.dp(8)
                            columnSpacing: units.dp(8)

                            Label {
                                text: qsTr("Content")
                                style: "subheading"
                            }
                            TextField {
                                id: loginContent
                                text: !newSite && currentSite.siteLogin.isStored ? Backend.decrypt(currentSite.siteLogin.content) : ""
                                placeholderText: !newSite && currentSite.siteLogin.isStored && text == "" ? qsTr("Encrypted with a different master key") : ""
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
            }

            View {
                width: units.dp(500)
                height: columnAnswer.implicitHeight + units.dp(32)

                elevation: 1

                ColumnLayout {
                    id: columnAnswer

                    anchors {
                        fill: parent
                        margins: units.dp(16)
                    }

                    GridLayout {
                        columns: 2
                        rowSpacing: units.dp(8)
                        columnSpacing: units.dp(8)

                        Label {
                            text: qsTr("Answer")
                            style: "subheading"
                        }
                        MenuField {
                            id: answerSelection
                            model: [qsTr("None"), qsTr("Generate"), qsTr("Store")]
                            selectedIndex: !newSite ? currentSite.siteAnswer.contentType : 0
                            Layout.fillWidth: true
                        }
                    }

                    ViewSwitcher {
                        anchors {
                            left: parent.left
                            right: parent.right
                        }

                        index: answerSelection.selectedIndex -1

                        Column {
                            anchors.fill: parent
                            spacing: units.dp(8)

                            GridLayout {
                                columns: 2
                                rowSpacing: units.dp(8)
                                columnSpacing: units.dp(8)

                                Label {
                                    text: qsTr("Context")
                                    style: "subheading"
                                }
                                TextField {
                                    id: answerContext
                                    text: !newSite && currentSite.siteAnswer.isGenerated ? currentSite.siteAnswer.context : ""
                                    Layout.fillWidth: true
                                }

                                Label {
                                    text: qsTr("Counter")
                                    style: "subheading"
                                }
                                TextField {
                                    id: answerCounter
                                    text: !newSite && currentSite.siteAnswer.isGenerated ? currentSite.siteAnswer.counter : 1
                                    input.inputMask: "000"
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

                                visible: answerSelection.selectedIndex == 1

                                Label {
                                    anchors.centerIn: parent
                                    style: "title"
                                    text: siteName.text != ""
                                          ? Backend.passwordForSite(siteName.text, MPW.typeWithNameAsInt(MPW.typeNamePhrase()),
                                                                    parseInt(answerCounter.text), MPW.variantNameAnswer(), answerContext.text)
                                          : qsTr("Preview")
                                    color: "white"
                                }
                            }
                        }

                        GridLayout {
                            anchors {
                                left: parent.left
                                right: parent.right
                            }

                            columns: 2
                            rowSpacing: units.dp(8)
                            columnSpacing: units.dp(8)

                            Label {
                                text: qsTr("Content")
                                style: "subheading"
                            }
                            TextField {
                                id: answerContent
                                text: !newSite && currentSite.siteAnswer.isStored ? Backend.decrypt(currentSite.siteAnswer.content) : ""
                                placeholderText: !newSite && currentSite.siteAnswer.isStored && text == "" ? qsTr("Encrypted with a different master key") : ""
                                Layout.fillWidth: true
                            }
                        }
                    }
                }
            }
        }
    }
}


