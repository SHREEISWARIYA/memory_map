#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include "shipdata.h"
#include "shipdatamodel.h"
#include "cursorcontroller.h"
#include "pastTrail.h"
#include "pasthistory.h"
//#include "messagetype.h"
#include "ShapefileManager.h"
#include "UserSettings.h"
#include "backgroundprocessor.h"


int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QApplication app(argc, argv);

    QQmlApplicationEngine engine;

    //qmlRegisterType<PastTrail>("com.example", 1, 0, "PastTrail");

    ShipData shipData;
    ShipDataModel shipDataModel;
    CursorController cursorController;
    //PastTrack pastTrack;
    PastTrail pastTrail;
    PastHistory pastHistory;
    UserSettings userSettings;
    BackgroundProcessor backgroundProcessor;

    // Connect the mapPathsUpdated signal to startProcessing slot
    QObject::connect(&userSettings, &UserSettings::mapPathsUpdated,
                     [&backgroundProcessor, &userSettings]() {
                         backgroundProcessor.startProcessing(&userSettings);
                     });

    //ShipTableModel *shipTableModel = new ShipTableModel();
    //MessageType *messageType = new MessageType();
    //shipTableModel->setMessageType(messageType);




    engine.rootContext()->setContextProperty("shipData", &shipData);
    engine.rootContext()->setContextProperty("shipDataModel", &shipDataModel);
    engine.rootContext()->setContextProperty("cursorController", &cursorController);
    //engine.rootContext()->setContextProperty("pastTrack", &pastTrack);
    engine.rootContext()->setContextProperty("pastTrail", &pastTrail);
    engine.rootContext()->setContextProperty("pastHistory", &pastHistory);
    //engine.rootContext()->setContextProperty("messageType", messageType);
    
    engine.rootContext()->setContextProperty("userSettings", &userSettings);
    engine.rootContext()->setContextProperty("backgroundProcessor", &backgroundProcessor);

    // Initial start of processing
    backgroundProcessor.startProcessing(&userSettings);

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
