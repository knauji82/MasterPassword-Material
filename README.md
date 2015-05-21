# MasterPassword-Material

MasterPassword-Material is a cross-platform GUI for the [Master Password](http://masterpasswordapp.com/) algorithm written with Qt and based on Google's Material Design.

Current version is 0.9.0

## About

This is a password manager where you can access all your passwords using only a single master password.
A unique password, username or security answer for each site is generated from the master password and other properties such as the name of the site and a counter. 
When using the algorithm, passwords are not stored so there is no password file that can get lost or stolen.

If you want to know how the algorithm works or to get more familiar with the concept, visit [masterpasswordapp.com](http://masterpasswordapp.com/algorithm.html).

### Features

* Material Design
* Generate passwords based on templates:
	* Maximum Security Password
	* Long Password
	* Medium Password
	* Basic Password
	* Short Password
	* PIN
* Generate usernames and logins
* Optionally store a password, login or answer encrypted
* Easily copy passwords to the clipboard
* Quick access to passwords from the system tray
* Use a shortcut or hotkey to open the app from the background
* Auto logout
* Multiple users
* Json import/export (compatible with  [Master Password for Android](https://play.google.com/store/apps/details?id=de.devland.masterpassword&hl=en))

## Installation

### Dependencies

* Qt5
* QtQuick
* [QML-Material](https://github.com/papyros/qml-material)

### Windows

Prebuilt binaries can be found under [releases](https://github.com/fosbob/MasterPassword-Material/releases).

## Build instructions

### Linux

```
./prepare
mkdir build && cd build
qmake ../masterpassword-material.pro -r -spec linux-g++
make
```

### Windows
```
prepare.bat
mkdir build
cd build
qmake.exe ../masterpassword-material.pro -r -spec win32-g++ "CONFIG += release"
mingw32-make -f Makefile.Release
```

## License

MasterPassword-Material is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
