#include "pastTrail.h"
#include <QUrlQuery>

PastTrail::PastTrail(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this, &PastTrail::onNetworkReply);
}

void PastTrail::fetchTrackHistory(const QString &uuid, int hours)
{
    QUrl url("http://localhost:3000/api/ships/track-history/" + uuid);
    QUrlQuery query;
    query.addQueryItem("hours", QString::number(hours));
    url.setQuery(query);

    QNetworkRequest request(url);
    m_manager->get(request);
}

void PastTrail::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonObject jsonObj = jsonDoc.object();

        QString uuid = jsonObj["uuid"].toString();
        QJsonArray trackHistoryArray = jsonObj["trackHistory"].toArray();

        QVariantList trackHistory;
        for (const QJsonValue &value : trackHistoryArray) {
            QJsonObject point = value.toObject();
            QVariantMap pointMap;
            pointMap["latitude"] = point["latitude"].toDouble();
            pointMap["longitude"] = point["longitude"].toDouble();
            pointMap["timestamp"] = point["timestamp"].toString();
            trackHistory.append(pointMap);
        }
        qDebug() << "Emitting trackHistoryFetched signal for UUID:" << uuid << trackHistory.length();
        m_history[uuid] = trackHistory;
        emit trackHistoryFetched(uuid, trackHistory);
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
}
