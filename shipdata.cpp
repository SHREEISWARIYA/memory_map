#include "shipdata.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkReply>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QDateTime>

ShipData::ShipData(QObject *parent)
    : QObject(parent)
    , m_networkManager(new QNetworkAccessManager(this))
    , m_totalShips(0)
{
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &ShipData::onNetworkReply);
}

void ShipData::fetchShips()
{
    QUrl url("http://192.168.1.12:3000/api/ships/trackList");
    QNetworkRequest request(url);
    qDebug() << "Fetching ships from URL:" << url.toString();
    m_networkManager->get(request);
}

void ShipData::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QJsonDocument jsonDoc = QJsonDocument::fromJson(reply->readAll());
        QJsonArray jsonArray = jsonDoc.array();

        qDebug() << "Received" << jsonArray.size() << "ships from API";

        m_ships.clear();
        m_mmsiUuidMap.clear();

        for (const QJsonValue &value : jsonArray) {
            QJsonObject obj = value.toObject();
            QString uuid = obj["uuid"].toString();

            // Handle MMSI as a number
            QString mmsi = QString::number(obj["mmsi"].toInt());

            // qDebug() << "Processing ship:";
            // qDebug() << "  UUID:" << uuid;
            // qDebug() << "  MMSI:" << mmsi;

            QVariantMap shipData;
            for (auto it = obj.constBegin(); it != obj.constEnd(); ++it) {
                if (it.key() == "mmsi") {
                    // Store MMSI as a string in shipData
                    shipData[it.key()] = QString::number(it.value().toInt());
                } else {
                    shipData[it.key()] = it.value().toVariant();
                }
            }

            m_ships[uuid] = shipData;
            if (!mmsi.isEmpty() && mmsi != "0") {
                m_mmsiUuidMap[mmsi] = uuid;
            } else {
                qDebug() << "  Warning: Invalid MMSI for ship with UUID:" << uuid;
            }
        }

        qDebug() << "Final m_mmsiUuidMap size:" << m_mmsiUuidMap.size();
        qDebug() << "Contents of m_mmsiUuidMap:";
        for (auto it = m_mmsiUuidMap.constBegin(); it != m_mmsiUuidMap.constEnd(); ++it) {
            //qDebug() << "MMSI:" << it.key() << "UUID:" << it.value();
        }

        m_totalShips = m_ships.count();
        emit totalShipsChanged();
        emit shipsChanged();
        emit mmsiUuidListChanged();
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }
    reply->deleteLater();
}

int ShipData::totalShips() const
{
    return m_totalShips;
}

QVariantList ShipData::shipList() const
{
    QVariantList list;
    for (auto it = m_ships.constBegin(); it != m_ships.constEnd(); ++it) {
        QVariantMap ship = it.value();
        ship["uuid"] = it.key();
        list.append(ship);
    }
    return list;
}

QVariantMap ShipData::isValidShip(const QString &uuid) const
{
    if (m_ships.contains(uuid) &&
        m_ships[uuid].contains("track_name") &&
        m_ships[uuid].contains("latitude") &&
        m_ships[uuid].contains("longitude"))
    {
        QVariantMap shipDetails = m_ships[uuid];
        shipDetails["uuid"] = uuid;
        return shipDetails;
    }
    return QVariantMap(); // Return an empty map if the ship is not valid
}

QVariantList ShipData::mmsiUuidList() const
{
    QVariantList list;
    for (auto it = m_mmsiUuidMap.constBegin(); it != m_mmsiUuidMap.constEnd(); ++it) {
        QVariantMap item;
        item["mmsi"] = it.key();
        item["uuid"] = it.value();
        list.append(item);
    }
    return list;
}

QString ShipData::getUuidFromMmsi(const QString &mmsi) const
{
    if (m_mmsiUuidMap.contains(mmsi)) {
        return m_mmsiUuidMap[mmsi];
    }
    return QString(); // Return an empty string if no matching UUID is found
}

QVariantMap ShipData::getShipDetails(const QString &uuid) const
{
    if (m_ships.contains(uuid)) {
        return m_ships[uuid];
    }
    return QVariantMap(); // Return an empty map if no matching ship is found
}

void ShipData::printShipDetails(const QString &uuid) const
{
    QVariantMap shipDetails = getShipDetails(uuid);
    if (!shipDetails.isEmpty()) {
        qDebug() << "Ship Details for UUID:" << uuid;
        for (auto it = shipDetails.constBegin(); it != shipDetails.constEnd(); ++it) {
            qDebug() << "  " << it.key() << ":" << it.value().toString();
        }
    } else {
        qDebug() << "No ship details found for UUID:" << uuid;
    }
}




