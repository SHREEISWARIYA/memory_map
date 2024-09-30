#include "ShipTableModel.h"

#include <QDateTime>
#include <QTimeZone>
#include <QJsonObject>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

ShipTableModel::ShipTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_networkManager = new QNetworkAccessManager(this);
    connect(m_networkManager, &QNetworkAccessManager::finished, this, &ShipTableModel::onNetworkReply);

    fetchMessageTypes();
    fetchTrackNavStatuses();
}

void ShipTableModel::fetchMessageTypes()
{
    QNetworkRequest request(QUrl("http://localhost:3000/api/ships/Message-Types"));
    m_networkManager->get(request);
}

void ShipTableModel::fetchTrackNavStatuses()
{
<<<<<<< HEAD
    QNetworkRequest request(QUrl("http://localhost:3000/api/ships/TrackNavStatuses"));
=======
    QNetworkRequest request(QUrl("http://192.168.1.12:3000/api/ships/TrackNavStatuses"));
>>>>>>> origin/master
    m_networkManager->get(request);
}

void ShipTableModel::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray response = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(response);
        QJsonArray jsonArray = jsonDoc.array();

        if (reply->url().toString().contains("Message-Types")) {
            for (const QJsonValue &value : jsonArray) {
                QJsonObject obj = value.toObject();
                int id = obj["id"].toInt();

                QVariantMap messageData;
                messageData["source_type"] = obj["source_type"].toString();
                messageData["source_sub_type"] = obj["source_sub_type"].toString();
                messageData["short_desc"] = obj["short_desc"].toString();
                messageData["description"] = obj["description"].toString();

                m_messageTypeMap[id] = messageData;
            }

            // Print all the data in the console
            QMapIterator<int, QVariantMap> i(m_messageTypeMap);
            // while (i.hasNext()) {
            //     i.next();
            //     qDebug() << "Message Type ID:" << i.key();
            //     qDebug() << "  Source Type:" << i.value()["source_type"].toString();
            //     qDebug() << "  Source Sub Type:" << i.value()["source_sub_type"].toString();
            //     qDebug() << "  Short Description:" << i.value()["short_desc"].toString();
            //     qDebug() << "  Description:" << i.value()["description"].toString();
            //     qDebug() << "";
            // }

            emit messageTypesLoaded();
        }
        else if (reply->url().toString().contains("TrackNavStatuses")) {
            for (const QJsonValue &value : jsonArray) {
                QJsonObject obj = value.toObject();
                int id = obj["id"].toInt();
                QString navStatus = obj["nav_status"].toString();

                m_trackNavStatusMap[id] = navStatus;
            }

            // Print all the data in the console
            QMapIterator<int, QString> i(m_trackNavStatusMap);
            // while (i.hasNext()) {
            //     i.next();
            //     qDebug() << "Track Nav Status ID:" << i.key();
            //     qDebug() << "  Navigation Status:" << i.value();
            //     qDebug() << "";
            // }

            emit trackNavStatusesLoaded();
        }
    } else {
        qDebug() << "Error fetching data:" << reply->errorString();
    }

    reply->deleteLater();
}

QString ShipTableModel::getMessageTypeDescription(int messageTypeId) const
{
    if (m_messageTypeMap.contains(messageTypeId)) {
        return m_messageTypeMap[messageTypeId]["description"].toString();
    }
    return QString();
}

// Add this new function
QString ShipTableModel::getTrackNavStatus(int statusId) const
{
    return m_trackNavStatusMap.value(statusId, "Unknown");
}


int ShipTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    
    int totalRows = m_shipOrder.size();
    int startIndex = (m_currentPage-1 ) * m_itemsPerPage;
    
    return qMin(m_itemsPerPage, totalRows - startIndex);
}


int ShipTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return m_headers.size();
}

// QVariant ShipTableModel::data(const QModelIndex &index, int role) const
// {
//     if (!index.isValid() || role != Qt::DisplayRole)
//         return QVariant();


//     int row = index.row();
//     int col = index.column();
//     int actualIndex = (m_currentPage - 1) * m_itemsPerPage + row;

//     if (actualIndex >= m_shipOrder.size())
//         return QVariant();

//     if (col < m_headers.size()) {
//         QString uuid = m_shipOrder[actualIndex];
//         QString key = m_headers[col];

//         // if (key == "message_type_id") {
//         //     int messageTypeId = m_shipData[uuid][key].toInt();
//         //     if (m_messageType) {
//         //         return m_messageType->getShortDesc(messageTypeId);
//         //     }
//         // }

//         if (key == "created_at" || key == "sensor_timestamp" || key == "updated_at") {
//             return formatTimestamp(m_shipData[uuid][key].toVariant());
//         }
//         else if (key == "latitude" || key == "longitude") {
//             QJsonValue value = m_shipData[uuid][key];
//             if (value.isDouble()) {
//                 double doubleValue = value.toDouble();
//                 QString dmsValue = decimalToDMS(doubleValue, key == "latitude");
//                 return dmsValue;
//             }
//         }
//         else if (key == "speed_over_ground" || key == "rate_of_turn" || key == "true_heading" ||
//                  key == "height_depth" || key == "message_type_id" || key == "track_nav_status_id") {
//             // Add any specific formatting for these columns if needed
//             return m_shipData[uuid][key].toVariant();
//         }
//         return m_shipData[uuid][key].toVariant();
//     }
//     else if (col == m_headers.size()) {
//         return QVariant("Action");
//     }

//     return QVariant();
// }


QVariant ShipTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    int row = index.row();
    int col = index.column();
    int actualIndex = (m_currentPage - 1) * m_itemsPerPage + row;

    if (actualIndex >= m_shipOrder.size())
        return QVariant();

    if (col < m_headers.size()) {
        QString uuid = m_shipOrder[actualIndex];
        QString key = m_headers[col];

        if (key == "message_type__id") {
            int messageTypeId = m_shipData[uuid][key].toInt();
            if (m_messageTypeMap.contains(messageTypeId)) {
                return m_messageTypeMap[messageTypeId]["short_desc"].toString();
            } else {
                return QString("Unknown (%1)").arg(messageTypeId);
            }
        }
        else if (key == "track_nav_status__id") {
            int statusId = m_shipData[uuid][key].toInt();
            return getTrackNavStatus(statusId);
        }
        else if (key == "created_at" || key == "sensor_timestamp" || key == "updated_at") {
            return formatTimestamp(m_shipData[uuid][key].toVariant());
        }
        else if (key == "latitude" || key == "longitude") {
            QJsonValue value = m_shipData[uuid][key];
            if (value.isDouble()) {
                double doubleValue = value.toDouble();
                QString dmsValue = decimalToDMS(doubleValue, key == "latitude");
                return dmsValue;
            }
        }
        else if (key == "speed_over_ground" || key == "rate_of_turn" || key == "true_heading" ||
                 key == "height_depth" || key == "track_nav_status__id") {
            // Add any specific formatting for these columns if needed
            return m_shipData[uuid][key].toVariant();
        }
        return m_shipData[uuid][key].toVariant();
    }
    else if (col == m_headers.size()) {
        return QVariant("Action");
    }

    return QVariant();
}




QVariant ShipTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal) {
        if (section < m_headers.size())
            return m_headers[section];
        // else if (section == m_headers.size())
        //     return "Actions"; // Header for the button column
    }

    return QVariant();
}


void ShipTableModel::setShipData(const QMap<QString, QJsonObject> &shipData, const QVector<QString> &shipOrder)
{
    beginResetModel();
    m_shipData = shipData;
    m_shipOrder = shipOrder;
    if (!m_shipData.isEmpty()) {
        m_headers = m_shipData.first().keys();
        // Ensure the columns are in the desired order
        QStringList orderedHeaders = {"mmsi", "track_name", "latitude", "longitude","speed_over_ground",
                                     "course_over_ground","rate_of_turn", "true_heading", "height_depth",
                                     "message_type__id", "track_nav_status__id"};
        for (const QString &header : orderedHeaders) {
            m_headers.removeAll(header);
        }
        // Add the ordered headers at the beginning
        for (int i = orderedHeaders.size() - 1; i >= 0; --i) {
            m_headers.prepend(orderedHeaders[i]);
        }
    }
    //qDebug() << "Headers set to:" << m_headers;
    endResetModel();
}

void ShipTableModel::setPage(int page, int itemsPerPage)
{
    beginResetModel();
    m_currentPage = page;
    m_itemsPerPage = itemsPerPage;
    endResetModel();
}

// Add this method to ShipTableModel class
QJsonObject ShipTableModel::getShipDataForRow(int row) const
{
    int actualIndex = (m_currentPage - 1) * m_itemsPerPage + row;
    if (actualIndex < m_shipOrder.size()) {
        QString uuid = m_shipOrder[actualIndex];
        return m_shipData[uuid];
    }
    return QJsonObject();
}

QString ShipTableModel::decimalToDMS(double decimal, bool isLatitude)
{
    int degrees = static_cast<int>(std::abs(decimal));
    double minutesDecimal = (std::abs(decimal) - degrees) * 60;
    int minutes = static_cast<int>(minutesDecimal);
    double seconds = (minutesDecimal - minutes) * 60;

    QString direction = isLatitude ?
                            (decimal >= 0 ? "N" : "S") :
                            (decimal >= 0 ? "E" : "W");

    return QString("%1°%2'%3\"%4")
        .arg(degrees)
        .arg(minutes, 2, 10, QChar('0'))
        .arg(seconds, 4, 'f', 1, QChar('0'))
        .arg(direction);
}

QString ShipTableModel::formatTimestamp(const QVariant &timestamp) const
{
    bool ok;
    qint64 msecsSinceEpoch = timestamp.toLongLong(&ok);

    QDateTime dateTime;
    if (ok) {
        dateTime = QDateTime::fromMSecsSinceEpoch(msecsSinceEpoch);
    } else {
        dateTime = QDateTime::fromString(timestamp.toString(), Qt::ISODate);
        if (!dateTime.isValid()) {
            QStringList formats = {"yyyy-MM-dd hh:mm:ss", "MM/dd/yyyy hh:mm:ss"};
            for (const QString &format : formats) {
                dateTime = QDateTime::fromString(timestamp.toString(), format);
                if (dateTime.isValid()) break;
            }
        }
    }

    if (!dateTime.isValid()) {
        //qWarning() << "Failed to parse timestamp:" << timestamp;
        return timestamp.toString();
    }

    // Convert to local time
    dateTime = dateTime.toLocalTime();

    // Format the date and time in a single line
    return dateTime.toString("dd MMM yyyy hh:mm:ss");
}

// QString ShipTableModel::getTrackNavStatus(int statusId) const
// {
//     return m_trackNavStatusMap.value(statusId, "Unknown");
// }
