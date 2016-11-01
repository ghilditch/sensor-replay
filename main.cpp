#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "workouts.h"
#include "workout.h"
#include "boneorientation.h"
#include "boneposition.h"
#include "jointangle.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    qmlRegisterType<Workouts>("Workout", 1,0, "Workouts");
    qmlRegisterType<Workout>("Workout", 1,0, "Workout");
    qmlRegisterType<JointAngle>("Workout", 1,0, "JointAngle");
    qmlRegisterType<BoneOrientation>("Workout", 1,0, "BoneOrientation");
    qmlRegisterType<BonePosition>("Workout", 1,0, "BonePosition");

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/Visualizer.qml")));

    return app.exec();
}
