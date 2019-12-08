RESOURCES += \
    $$PWD/com_bckmnn_properties.qrc

!contains(PROPERTIES_CONFIG,"noautoregister") {
    DEFINES += PROPERTIES_AUTO_REGISTER
}

HEADERS += \
    $$PWD/src/propertiesattachedtype.h \
    $$PWD/src/propertymanager.h \
    $$PWD/src/propertytypes.h

SOURCES += \
    $$PWD/src/properties_autoregister.cpp \
    $$PWD/src/propertiesattachedtype.cpp \
    $$PWD/src/propertymanager.cpp

DISTFILES += \
    $$PWD/README.md \
    $$PWD/LICENSE
