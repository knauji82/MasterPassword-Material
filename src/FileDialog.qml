/**
 * FileDialog.qml
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
import QtQuick.Dialogs 1.0
import Qt.labs.folderlistmodel 2.1

FileDialog {
    property string filePath: {
        var path = fileUrl.toString()
        if (Qt.platform.os == "linux")
            path = path.replace(/^(file:\/{2})/, "")
        else
            path = path.replace(/^(file:\/{3})/, "")
        decodeURIComponent(path)
    }

    property string folderPath: {
        var path = folder.toString()
        if (Qt.platform.os == "linux")
            path = path.replace(/^(file:\/{2})/, "")
        else
            path = path.replace(/^(file:\/{3})/, "")
        decodeURIComponent(path)
    }
}

