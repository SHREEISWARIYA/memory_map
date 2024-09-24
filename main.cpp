#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "shipdata.h"
#include "shipdatamodel.h"
#include "cursorcontroller.h"
#include "pastTrack.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    ShipData shipData;
    ShipDataModel shipDataModel;
    CursorController cursorController;
    PastTrack pastTrack;

    engine.rootContext()->setContextProperty("shipData", &shipData);
    engine.rootContext()->setContextProperty("shipDataModel", &shipDataModel);
    engine.rootContext()->setContextProperty("cursorController", &cursorController);
    engine.rootContext()->setContextProperty("pastTrack", &pastTrack);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
