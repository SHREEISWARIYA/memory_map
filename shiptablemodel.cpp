#include "ShipTableModel.h"

ShipTableModel::ShipTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
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
        //qDebug() << "Accessing data for key:" << key << "uuid:" << uuid;
        if (key == "latitude" || key == "longitude") {
            QJsonValue value = m_shipData[uuid][key];
            if (value.isDouble()) {
                double doubleValue = value.toDouble();
                QString dmsValue = decimalToDMS(doubleValue, key == "latitude");
                qDebug() << "Converting" << key << doubleValue << "to DMS:" << dmsValue;
                return dmsValue;
            } else {
                qDebug() << "Failed to convert" << key << "to double. Value:" << value.toString();
            }
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

// void ShipTableModel::setShipData(const QMap<QString, QJsonObject> &shipData, const QVector<QString> &shipOrder)
// {
//     beginResetModel();
//     m_shipData = shipData;
//     m_shipOrder = shipOrder;
//     if (!m_shipData.isEmpty()) {
//         m_headers = m_shipData.first().keys().toVector();
//     }
//     endResetModel();
// }

// Modify the setShipData function to set the headers
void ShipTableModel::setShipData(const QMap<QString, QJsonObject> &shipData, const QVector<QString> &shipOrder)
{
    beginResetModel();
    m_shipData = shipData;
    m_shipOrder = shipOrder;
    if (!m_shipData.isEmpty()) {
        m_headers = m_shipData.first().keys();
        // Ensure MMSI is the first column, track_name is the second, and latitude/longitude are third and fourth
        m_headers.removeAll("mmsi");
        m_headers.removeAll("track_name");
        m_headers.removeAll("latitude");
        m_headers.removeAll("longitude");
        m_headers.prepend("longitude");
        m_headers.prepend("latitude");
        m_headers.prepend("track_name");
        m_headers.prepend("mmsi");
    }
    qDebug() << "Headers set to:" << m_headers;
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
