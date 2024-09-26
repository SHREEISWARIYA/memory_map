#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "shipdata.h"
#include "shipdatamodel.h"
#include "cursorcontroller.h"
//#include "pastTrack.h"
#include "pastTrail.h"
#include "pasthistory.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //qmlRegisterType<PastTrail>("com.example", 1, 0, "PastTrail");

    ShipData shipData;
    ShipDataModel shipDataModel;
    CursorController cursorController;
    //PastTrack pastTrack;
    PastTrail pastTrail;
    PastHistory pastHistory;

    //qmlRegisterType<PastTrail>("com.pastTrail", 1, 0, "PastTrail");

    engine.rootContext()->setContextProperty("shipData", &shipData);
    engine.rootContext()->setContextProperty("shipDataModel", &shipDataModel);
    engine.rootContext()->setContextProperty("cursorController", &cursorController);
    //engine.rootContext()->setContextProperty("pastTrack", &pastTrack);
    engine.rootContext()->setContextProperty("pastTrail", &pastTrail);
    engine.rootContext()->setContextProperty("pastHistory", &pastHistory);


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
