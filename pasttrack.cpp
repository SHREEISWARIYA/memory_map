// pastTrack.cpp
#include "pastTrack.h"
#include <QNetworkRequest>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

PastTrack::PastTrack(QObject *parent) : QObject(parent)
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &PastTrack::onTrackHistoryFetched);
}

void PastTrack::fetchTrackHistory(const QString &uuid, int hours)
{
    QUrl url(QString("http://localhost:3000/api/ships/track-history/%1?hours=%2").arg(uuid).arg(hours));
    QNetworkRequest request(url);
    networkManager->get(request);
    qDebug() << "***  Fetching track history for UUID: ***" << uuid << "with hours:" << hours;
}

void PastTrack::onTrackHistoryFetched(QNetworkReply *reply)
{
    qDebug() << "onTrackHistoryFetched called";
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QVariantMap trackHistory = jsonDoc.toVariant().toMap();
        qDebug() << " *** Emitting trackHistoryFetched signal ***";
        memoryMap.insert(trackHistory["uuid"].toString(), trackHistory); // Store data in memory map
        printTrackHistory(trackHistory); // Print track history in readable form

        emit trackHistoryFetched(trackHistory);

    } else {
        qWarning() << "Error fetching track history:" << reply->errorString();
    }
    reply->deleteLater();
}

void PastTrack::printTrackHistory(const QVariantMap &trackHistory)
{
    qDebug() << "Track history for UUID:" << trackHistory["uuid"].toString();
    qDebug() << "Hours:" << trackHistory["hours"].toInt();
    QVariantList trackList = trackHistory["trackHistory"].toList();
    for (const QVariant &track : trackList) {
        QVariantMap trackMap = track.toMap();
        qDebug() << "Latitude:" << trackMap["latitude"].toDouble()
                 << "Longitude:" << trackMap["longitude"].toDouble()
                 << "Speed Over Ground:" << trackMap["speed_over_ground"].toInt()
                 << "Course Over Ground:" << trackMap["course_over_ground"].toInt()
                 << "True Heading:" << trackMap["true_heading"].toInt()
                 << "Rate of Turn:" << trackMap["rate_of_turn"].toInt()
                 << "Sensor Timestamp:" << trackMap["sensor_timestamp"].toString();
    }
}
