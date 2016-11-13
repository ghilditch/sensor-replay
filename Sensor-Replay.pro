TEMPLATE = app

QT += qml quick

CONFIG += c++11

SOURCES += main.cpp \
    workout.cpp \
    workouts.cpp \
    sensorfile.cpp \
    notchworkout.cpp \
    bone.cpp \
    boneposition.cpp \
    boneorientation.cpp \
    csvreader.cpp \
    notchsensorsample.cpp \
    skeleton.cpp \
    vector.cpp \
    playbackengine.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = .\

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    InfoSheet.qml \
    ControlButton.qml \
    FpsDisplay.qml \
    StyledSlider.qml

HEADERS += \
    workout.h \
    workouts.h \
    sensorfile.h \
    notchworkout.h \
    bone.h \
    boneposition.h \
    boneorientation.h \
    csvreader.h \
    notchsensorsample.h \
    skeleton.h \
    vector.h \
    rendercontext.h \
    playbackengine.h
