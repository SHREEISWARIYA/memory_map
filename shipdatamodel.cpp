#include "shipdatamodel.h"
#include <QJsonDocument>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonArray>
#include <QDebug>

ShipDataModel::ShipDataModel(QObject *parent)
    : QObject(parent)
    , m_tableModel(new ShipTableModel(this))
{
}

void ShipDataModel::fetchShipData()
{
    m_isLoading = true;
    emit isLoadingChanged();

    QNetworkAccessManager *manager = new QNetworkAccessManager(this);
    QNetworkRequest request(QUrl("http://localhost:3000/api/ships/trackList"));
    QNetworkReply *reply = manager->get(request);

    connect(reply, &QNetworkReply::finished, this, [this, reply]() {
        m_isLoading = false;
        emit isLoadingChanged();

        if (reply->error() == QNetworkReply::NoError) {
            QString response = reply->readAll();
            QJsonDocument jsonDoc = QJsonDocument::fromJson(response.toUtf8());
            //QJsonArray dataArray = jsonDoc["data"].toArray();
            QJsonArray dataArray = jsonDoc.array();


            m_shipData.clear();
            m_shipOrder.clear();  // Clear the order vector
            for (const QJsonValue &value : dataArray) {
                QJsonObject obj = value.toObject();
                QString uuid = obj["uuid"].toString();
                addShip(uuid, obj);
            }
            qDebug() << "Fetched" << m_shipOrder.size() << "ships";
            m_tableModel->setShipData(m_shipData, m_shipOrder);
            emit totalShipsChanged();
            updatePage();
        } else {
            qDebug() << "Error fetching ship data:" << reply->errorString();
        }

        reply->deleteLater();
        sender()->deleteLater();
    });
}

void ShipDataModel::addShip(const QString &uuid, const QJsonObject &shipData)
{
    m_shipData[uuid] = shipData;
    m_shipOrder.append(uuid);
}

void ShipDataModel::setCurrentPage(int page)
{
    if (m_currentPage != page) {
        m_currentPage = page;
        emit currentPageChanged();
        updatePage();
    }
}

void ShipDataModel::setItemsPerPage(int items)
{
    if (m_itemsPerPage != items) {
        m_itemsPerPage = items;
        emit itemsPerPageChanged();
        updatePage();
    }
}

void ShipDataModel::updatePage()
{
    m_tableModel->setPage(m_currentPage, m_itemsPerPage);
}



//******************************************************************************************************************************

// #include "shipdatamodel.h"
// #include <QJsonDocument>
// #include <QNetworkAccessManager>
// #include <QNetworkReply>
// #include <QJsonArray>
// #include <QDebug>

// ShipDataModel::ShipDataModel(QObject *parent)
//     : QObject(parent)
// {
// }

// void ShipDataModel::fetchShipData()
// {
//     m_isLoading = true;
//     emit isLoadingChanged();

//     QNetworkAccessManager *manager = new QNetworkAccessManager(this);
//     QNetworkRequest request(QUrl("http://localhost:3000/api/ships"));
//     QNetworkReply *reply = manager->get(request);

//     connect(reply, &QNetworkReply::finished, this, [this, reply]() {
//         m_isLoading = false;
//         emit isLoadingChanged();

//         if (reply->error() == QNetworkReply::NoError) {
//             QString response = reply->readAll();
//             QJsonDocument jsonDoc = QJsonDocument::fromJson(response.toUtf8());
//             QJsonArray dataArray = jsonDoc["data"].toArray();

//             m_shipData.clear();
//             for (const QJsonValue &value : dataArray) {
//                 QJsonObject obj = value.toObject();
//                 QString uuid = obj["uuid"].toString();
//                 addShip(uuid, obj);
//             }
//             qDebug() << "Fetched" << m_shipData.size() << "ships";
//         } else {
//             qDebug() << "Error fetching ship data:" << reply->errorString();
//         }

//         reply->deleteLater();
//         sender()->deleteLater();
//     });
// }

// void ShipDataModel::addShip(const QString &uuid, const QJsonObject &shipData)
// {
//     m_shipData[uuid] = shipData;
//     emit shipDataChanged();
// }

// QString ShipDataModel::getShipData() const
// {
//     QJsonObject result;
//     for (auto it = m_shipData.constBegin(); it != m_shipData.constEnd(); ++it) {
//         result[it.key()] = it.value();
//     }
//     QJsonDocument doc(result);
//     return doc.toJson(QJsonDocument::Indented);
// }

