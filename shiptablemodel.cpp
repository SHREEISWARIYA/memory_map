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
    int startIndex = (m_currentPage - 1) * m_itemsPerPage;
    
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
    if (!index.isValid())
        return QVariant();

    int row = index.row();
    int col = index.column();
    int actualIndex = (m_currentPage - 1) * m_itemsPerPage + row;

    if (actualIndex >= m_shipOrder.size())
        return QVariant();

    if (col < m_headers.size()) {
        QString uuid = m_shipOrder[actualIndex];
        QString key = m_headers[col];
        return m_shipData[uuid][key].toVariant();
    } else if (col == m_headers.size()) {
        // This is our button column
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
        // Ensure MMSI is the first column
        m_headers.removeAll("mmsi");
        m_headers.prepend("mmsi");
    }
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
