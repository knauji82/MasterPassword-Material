/**
 * main.qml
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

ApplicationWindow {
    title: "Master Password"
    visible: true
    width: Settings.value("window_width", 970)
    height: Settings.value("window_height", 560)
    minimumWidth: 460
    minimumHeight: 400

    initialPage: Qt.resolvedUrl("LoginPage.qml")

    theme {
        primaryColor: Palette.colors.blueGrey["500"]
        primaryDarkColor: Palette.colors.blueGrey["700"]
        accentColor: Palette.colors.teal["500"]
        backgroundColor: "#f3f3f3"
    }

    onClosing: {
        if (Backend.loggedIn()) {
            close.accepted = false
            hide()
        }
    }
}

