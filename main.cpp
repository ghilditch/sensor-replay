#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "workouts.h"
#include "workout.h"
#include "boneorientation.h"
#include "boneposition.h"
#include "notchsensorsample.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Workouts>("Workout", 1,0, "Workouts");
    qmlRegisterType<Workout>("Workout", 1,0, "Workout");
    qmlRegisterType<BoneOrientation>("Workout", 1,0, "BoneOrientation");
    qmlRegisterType<BonePosition>("Workout", 1,0, "BonePosition");
    qmlRegisterType<NotchSensorSample>("Workout", 1,0, "NotchSensorSample");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/Visualizer.qml")));

    return app.exec();
}
