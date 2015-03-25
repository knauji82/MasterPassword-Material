/**
 * LoginPage.qml
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
    title: qsTr("Login")

    function tryLogin() {
        if (user.text != "" && password.text != "")
        {
            if (Backend.login(user.text, password.text)) {
                pageStack.push(Qt.resolvedUrl("MainPage.qml"))
                pageStack.currentItem.title = user.text
            }
            else {
                errorDialog.text = qsTr("Wrong password")
                errorDialog.open()
            }
        }
        else if (user.text == "") {
            errorDialog.text = qsTr("Username is empty")
            errorDialog.open()
        }
        else if (password.text == "") {
            errorDialog.text = qsTr("Password is empty")
            errorDialog.open()
        }

        password.text = ""
    }

    function focusField() {
        if (user.text == "")
            user.focus = true
        else
            password.focus = true
    }

    Dialog {
        id: errorDialog
        title: qsTr("Error")
    }

    View {
        anchors.centerIn: parent

        width: units.dp(500)
        height: units.dp(240)

        elevation: 1

        ColumnLayout {
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
                    id: user
                    text: Settings.userName()
                    placeholderText: qsTr("Your username")
                    Layout.fillWidth: true
                }

                Label {
                    text: qsTr("Password")
                    style: "subheading"
                }
                TextField {
                    id: password
                    placeholderText: qsTr("Your Master Password")
                    input.echoMode: TextInput.Password
                    Layout.fillWidth: true
                }

                Component.onCompleted: focusField()
            }

            Button {
                text: qsTr("Login")
                textColor: theme.primaryColor
                elevation: 1
                Layout.fillWidth: true
                onClicked: tryLogin()
            }

            Keys.onReturnPressed: tryLogin()
            Keys.onEnterPressed: tryLogin()
        }
    }
}
