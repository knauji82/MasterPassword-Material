/**
 * SettingsPage.qml
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
import MasterPassword 1.0

Page {
    title: qsTr("Settings")

    function save() {
        Settings.setDefaultSiteType(MPW.typeWithNameAsInt(defaultSiteType.selectedText))
        Settings.setClipboardDuration(clipboardDuration.value)
        Settings.setAutoLogout(autoLogout.checked)
        Settings.setAutoLogoutDuration(autoLogoutDuration.value)
        Settings.setHidePasswords(hidePasswords.checked)
        Settings.setVerifyPassword(verifyPassword.checked)
        // finally set algorithm version to prevent problems with logout
        Settings.setAlgorithmVersion(algorithmVersion.selectedIndex)
    }

    View {
        anchors.centerIn: parent

        width: column.implicitWidth + Units.dp(32)
        height: column.implicitHeight + Units.dp(32)

        elevation: 1

        ColumnLayout {
            id: column

            anchors {
                fill: parent
                margins: Units.dp(16)
            }

            GridLayout {
                columns: 2
                rowSpacing: 10
                columnSpacing: 10

                Label {
                    text: qsTr("Algorithm version")
                }
                MenuField {
                    id: algorithmVersion
                    model: MPW.algorithmVersions()
                    selectedIndex: Settings.algorithmVersion()
                }

                Label {
                    text: qsTr("Default password type")
                }
                MenuField {
                    id: defaultSiteType
                    readonly property var typeNames: MPW.passwordTypeNames()
                    model: typeNames
                    selectedIndex: typeNames.indexOf(MPW.typeName(Settings.defaultSiteType()))
                    maxVisibleItems: typeNames.length
                    Layout.fillWidth: true
                }
                Label {
                    text: qsTr("Copy passwords for")
                }
                RowLayout {
                    NumberField {
                        id: clipboardDuration
                        value: Settings.clipboardDuration()
                        maxValue: 60
                        minValue: 1
                    }
                    Label {
                        text: qsTr("seconds")
                    }
                }

                Label {
                    text: qsTr("Auto logout")
                }
                CheckBox {
                    id: autoLogout
                    checked: Settings.autoLogout()
                }

                Label {
                    text: qsTr("Auto logout after")
                    enabled: autoLogout
                }
                RowLayout {
                    enabled: autoLogout.checked
                    NumberField {
                        id: autoLogoutDuration
                        value: Settings.autoLogoutDuration()
                        minValue: 1
                    }
                    Label {
                        text: qsTr("minutes")
                    }
                }

                Label {
                    text: qsTr("Hide passwords")
                }
                CheckBox {
                    id: hidePasswords
                    checked: Settings.hidePasswords()
                }

                Label {
                    text: qsTr("Verify password at login")
                }
                RowLayout {
                    CheckBox {
                        id: verifyPassword
                        checked: Settings.verifyPassword()
                        onCheckedChanged: if (checked && !Settings.verifyPassword()) {
                            passwordDialog.open()
                        }
                    }
                    Button {
                        visible: verifyPassword.checked
                        text: qsTr("Set password")
                        onClicked: passwordDialog.open()

                        Dialog {
                            id: passwordDialog
                            title: qsTr("Enter password")

                            function close() {}
                            function myClose() {
                                showing = false
                                parent.currentOverlay = null
                            }

                            GridLayout {
                                columns: 2
                                rowSpacing: 10
                                columnSpacing: 10

                                Label {
                                    text: qsTr("Password")
                                }
                                TextField {
                                    id: password1
                                    Layout.fillWidth: true
                                    echoMode: TextInput.Password
                                    focus: true
                                }

                                Label {
                                    text: qsTr("Confirm Password")
                                }
                                TextField {
                                    id: password2
                                    Layout.fillWidth: true
                                    echoMode: TextInput.Password
                                }
                            }

                            Text {
                                anchors.horizontalCenter: parent.horizontalCenter
                                text: (password2.focus &&
                                       password2.text !== "" &&
                                       password2.text !== password1.text)
                                     ? qsTr("Passwords do not match") : ""
                            }

                            onRejected: {
                                if (!Settings.verifyPassword())
                                    verifyPassword.checked = false
                                myClose()
                            }

                            onAccepted: if (password1.text !== "" && password1.text === password2.text) {
                                Settings.setPasswordHash(Backend.hash(password1.text))
                                myClose()
                            }
                        }
                    }
                }
            }

            RowLayout {
                anchors {
                    right: parent.right
                    left: parent.left
                }

                Button {
                    anchors.right: btn_save.left
                    text: qsTr("Cancel")
                    onClicked: pageStack.pop()
                }

                Button {
                    id: btn_save
                    anchors.right: parent.right
                    text: qsTr("Save")
                    onClicked: {
                        save()
                        pageStack.pop()
                    }
                }
            }
        }
    }
}
