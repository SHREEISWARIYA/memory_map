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
    if (!index.isValid() || role != Qt::DisplayRole)
        return QVariant();

    int row = index.row();
    int col = index.column();
    int actualIndex = (m_currentPage - 1) * m_itemsPerPage + row;

    if (actualIndex >= m_shipOrder.size())
        return QVariant();

    QString uuid = m_shipOrder[actualIndex];
    QString key = m_headers[col];

    return m_shipData[uuid][key].toVariant();
}

QVariant ShipTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (orientation == Qt::Horizontal && section < m_headers.size())
        return m_headers[section];

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
