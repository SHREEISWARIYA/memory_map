#include "messagetype.h"
#include <QDebug>

MessageType::MessageType(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    connect(manager, &QNetworkAccessManager::finished, this, &MessageType::onNetworkReply);
}

void MessageType::fetchMessageTypes()
{
    QNetworkRequest request(QUrl("http://localhost:3000/api/ships/Message-Types"));
    manager->get(request);
}

void MessageType::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonArray jsonArray = jsonDoc.array();

        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();
            int id = obj["id"].toInt();

            QVariantMap messageData;
            messageData["source_type"] = obj["source_type"].toString();
            messageData["source_sub_type"] = obj["source_sub_type"].toString();
            messageData["short_desc"] = obj["short_desc"].toString();
            messageData["description"] = obj["description"].toString();

            messageTypeMap[id] = messageData;
        }

        // Print all the data in the console
        QMapIterator<int, QVariantMap> i(messageTypeMap);
        while (i.hasNext()) {
            i.next();
            qDebug() << "ID:" << i.key();
            qDebug() << "  Source Type:" << i.value()["source_type"].toString();
            qDebug() << "  Source Sub Type:" << i.value()["source_sub_type"].toString();
            qDebug() << "  Short Description:" << i.value()["short_desc"].toString();
            qDebug() << "  Description:" << i.value()["description"].toString();
            qDebug() << "";
        }

        emit messageTypesLoaded();
    } else {
        qDebug() << "Error:" << reply->errorString();
    }

    reply->deleteLater();
}
