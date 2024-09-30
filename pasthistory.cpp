// pasthistory.cpp
#include "pasthistory.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QUrlQuery>

PastHistory::PastHistory(QObject *parent) : QObject(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &PastHistory::onNetworkReply);
}

void PastHistory::fetchTrackHistory(const QString &shipUuid, int hours)
{
    QUrl url(QString("http://192.168.1.12:3000/api/ships/track-history/%1").arg(shipUuid));
    QUrlQuery query;
    query.addQueryItem("hours", QString::number(hours));
    url.setQuery(query);

    QNetworkRequest request(url);
    m_networkManager->get(request);
}

void PastHistory::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonArray jsonArray = jsonDoc.array();

        QVariantList trackHistory;
        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();
            trackHistory.append(QVariantMap{
                {"latitude", obj["latitude"].toDouble()},
                {"longitude", obj["longitude"].toDouble()},
                {"timestamp", obj["timestamp"].toString()}
            });
        }

        QString shipUuid = reply->url().path().split('/').last();
        m_trackHistoryMap[shipUuid] = trackHistory;

        emit trackHistoryFetched(shipUuid);
    } else {
        qDebug() << "Error fetching track history:" << reply->errorString();
    }

    reply->deleteLater();
}

QVariantList PastHistory::getTrackHistory(const QString &shipUuid) const
{
    return m_trackHistoryMap.value(shipUuid);
}
