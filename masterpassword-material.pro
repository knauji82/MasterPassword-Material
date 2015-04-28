TEMPLATE = app

VERSION = 0.9.0

TARGET = "MasterPassword"

QT += qml quick widgets svg

CONFIG += c++11

QMAKE_CFLAGS += -std=c11

INCLUDEPATH += $$PWD/lib/include

LIBS += \
    $$PWD/lib/scrypt/scrypt-crypto_aesctr.o \
    $$PWD/lib/scrypt/scrypt-sha256.o \
    $$PWD/lib/scrypt/scrypt-crypto_scrypt-nosse.o \
    $$PWD/lib/scrypt/scrypt-memlimit.o \
    $$PWD/lib/scrypt/scrypt-scryptenc_cpuperf.o \
    $$PWD/lib/scrypt/scrypt-scryptenc.o \
    -L$$PWD/lib/scrypt

DEFINES += APP_VERSION=\\\"$$VERSION\\\"

win32 {
    #CONFIG += static
    contains(QT_CONFIG, static) {
        QMAKE_LFLAGS += -static-libgcc -static-libstdc++
    }
    LIBS += \
        -LC:/OpenSSL-Win32/lib/MinGW \
        -leay32 \
        -lwsock32
    DEFINES += BUILDTIME=\\\"$$system('echo %time%')\\\"
    DEFINES += BUILDDATE=\\\"$$system('echo %date%')\\\"
    RC_ICONS += res/icon.ico
}
else {
    LIBS += -lcrypto
    DEFINES += BUILDTIME=\\\"$$system(date '+%H:%M:%S')\\\"
    DEFINES += BUILDDATE=\\\"$$system(date '+%d.%m.%Y')\\\"
}


HEADERS += \
    lib/masterpassword-algorithm/mpw-types.h \
    lib/masterpassword-algorithm/mpw-util.h \
    lib/masterpassword-algorithm/mpw-algorithm.h \
    src/masterpasswordapp.h \
    src/qmlapplicationengine.h \
    src/settings.h \
    src/content.h \
    src/site.h \
    src/sitemodel.h \
    src/siteproxymodel.h \
    src/categorymodel.h \
    src/sitehistorymodel.h \
    src/sitemenu.h \
    src/mpw.h \
    src/masterkey.h \
    src/clipboard.h \
    src/importer.h \
    src/exporter.h \
    src/simplecrypt.h

SOURCES += \
    lib/masterpassword-algorithm/mpw-types.c \
    lib/masterpassword-algorithm/mpw-util.c \
    lib/masterpassword-algorithm/mpw-algorithm.c \
    src/masterkey.cc \
    src/main.cc \
    src/masterpasswordapp.cc \
    src/qmlapplicationengine.cc \
    src/settings.cc \
    src/content.cc \
    src/sitemodel.cc \
    src/siteproxymodel.cc \
    src/categorymodel.cc \
    src/sitehistorymodel.cc \
    src/mpw.cc \
    src/clipboard.cc \
    src/importer.cc \
    src/exporter.cc \
    src/simplecrypt.cc

TRANSLATIONS = \
    lang/de_DE.ts

RESOURCES += resources.qrc

lupdate_only {
    SOURCES += src/*.qml
}
