QT += quick concurrent widgets
QTPLUGIN += qml_QtGraphicalEffects

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        BackgroundProcessor.cpp \
        UserSettings.cpp \
        cursorcontroller.cpp \
        main.cpp \
        messagetype.cpp \
        pasthistory.cpp \
        pasttrail.cpp \
        shipdata.cpp \
        shipdatamodel.cpp \
        shiptablemodel.cpp \
        EncConvertor.cpp \
        ShapefileManager.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += C:/OSGeo4W/include
LIBS += -LC:\OSGeo4W\lib -lgdal_i

HEADERS += \
    BackgroundProcessor.h \
    UserSettings.h \
    cursorcontroller.h \
    messagetype.h \
    pasthistory.h \
    pasttrail.h \
    shipdata.h \
    shipdatamodel.h \
    shiptablemodel.h \
    EncConvertor.h \
    ShapefileManager.h
