RESOURCES += \
    $$PWD/com_bckmnn_properties.qrc

!contains(PROPERTIES_CONFIG,"noautoregister") {
    DEFINES += PROPERTIES_AUTO_REGISTER
}

HEADERS += \
    $$PWD/src/propertiesattachedtype.h \
    $$PWD/src/propertymanager.h \
    $$PWD/src/propertysettings.h \
    $$PWD/src/propertytypes.h

SOURCES += \
    $$PWD/src/properties_autoregister.cpp \
    $$PWD/src/propertiesattachedtype.cpp \
    $$PWD/src/propertymanager.cpp \
    $$PWD/src/propertysettings.cpp

DISTFILES += \
    $$PWD/README.md \
    $$PWD/LICENSE
