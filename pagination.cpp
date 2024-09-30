#include "pagination.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>

Pagination::Pagination(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this, &Pagination::onNetworkReply);

    QNetworkRequest request(QUrl("http://localhost:3000/api/ships"));
    qDebug() << "Sending network request to:" << request.url().toString();
    m_manager->get(request);
}

QMap<QString, QVariantMap> Pagination::ships() const
{
    qDebug() << "ships() called, returning" << m_pagination.size() << "ships";
    return m_pagination;
}

void Pagination::printShipDetails(const QString &uuid) const
{
    if (m_pagination.contains(uuid)) {
        const QVariantMap &shipDetails = m_pagination[uuid];
        qDebug() << "Ship Details for UUID:" << uuid;
        for (auto it = shipDetails.constBegin(); it != shipDetails.constEnd(); ++it) {
            qDebug() << "  " << it.key() << ":" << it.value().toString();
        }
    } else {
        qDebug() << "No ship details found for UUID:" << uuid;
    }
    qDebug() << "***************************************************************************";
}

void Pagination::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonObject jsonObj = jsonDoc.object();
        QJsonArray dataArray = jsonObj["data"].toArray();

        qDebug() << "Received data array size:" << dataArray.size();

        m_pagination.clear();  // Clear existing data

        for (const QJsonValue &value : dataArray) {
            QJsonObject ship = value.toObject();
            QString uuid = ship["uuid"].toString();
            m_pagination[uuid] = ship.toVariantMap();
        }

        qDebug() << "Stored ships count:" << m_pagination.size();

        emit shipsChanged();

        // Print to console for debugging
        qDebug() << "Ships data in C++:";
        for (const auto &uuid : m_pagination.keys()) {
            printShipDetails(uuid);
            qDebug() << "---";
        }
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
}
