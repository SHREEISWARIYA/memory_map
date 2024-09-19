#ifndef SHIPDATAMODEL_H
#define SHIPDATAMODEL_H

#include <QObject>
#include <QMap>
#include <QJsonObject>
#include "ShipTableModel.h"

class ShipDataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)
    Q_PROPERTY(int totalShips READ totalShips NOTIFY totalShipsChanged)
    Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
    Q_PROPERTY(int itemsPerPage READ itemsPerPage WRITE setItemsPerPage NOTIFY itemsPerPageChanged)
    Q_PROPERTY(ShipTableModel* tableModel READ tableModel CONSTANT)

public:
    explicit ShipDataModel(QObject *parent = nullptr);

    Q_INVOKABLE void fetchShipData();
    Q_INVOKABLE void updatePage();

    bool isLoading() const { return m_isLoading; }
    int totalShips() const { return m_shipOrder.size(); }
    int currentPage() const { return m_currentPage; }
    void setCurrentPage(int page);
    int itemsPerPage() const { return m_itemsPerPage; }
    void setItemsPerPage(int items);
    ShipTableModel* tableModel() const { return m_tableModel; }

signals:
    void isLoadingChanged();
    void totalShipsChanged();
    void currentPageChanged();
    void itemsPerPageChanged();

private:
    void addShip(const QString &uuid, const QJsonObject &shipData);

    QMap<QString, QJsonObject> m_shipData;
    QVector<QString> m_shipOrder;
    bool m_isLoading = false;
    int m_currentPage = 1;
    int m_itemsPerPage = 10;
    ShipTableModel* m_tableModel;
};

#endif // SHIPDATAMODEL_H











//*************************************************************************************************************

// #ifndef SHIPDATAMODEL_H
// #define SHIPDATAMODEL_H

// #include <QObject>
// #include <QMap>
// #include <QVector>
// #include <QJsonObject>
// #include <QJsonArray>

// class ShipDataModel : public QObject
// {
//     Q_OBJECT
//     Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)
//     Q_PROPERTY(int totalShips READ totalShips NOTIFY totalShipsChanged)
//     Q_PROPERTY(int currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged)
//     Q_PROPERTY(int itemsPerPage READ itemsPerPage WRITE setItemsPerPage NOTIFY itemsPerPageChanged)
//     Q_PROPERTY(QJsonArray paginatedData READ paginatedData NOTIFY paginatedDataChanged)

// public:
//     explicit ShipDataModel(QObject *parent = nullptr);

//     Q_INVOKABLE void fetchShipData();
//     Q_INVOKABLE void updatePaginatedData();

//     bool isLoading() const { return m_isLoading; }
//     int totalShips() const { return m_shipData.size(); }
//     int currentPage() const { return m_currentPage; }
//     void setCurrentPage(int page);
//     int itemsPerPage() const { return m_itemsPerPage; }
//     void setItemsPerPage(int items);
//     QJsonArray paginatedData() const { return m_paginatedData; }

// signals:
//     void isLoadingChanged();
//     void totalShipsChanged();
//     void currentPageChanged();
//     void itemsPerPageChanged();
//     void paginatedDataChanged();

// private:
//     void addShip(const QString &uuid, const QJsonObject &shipData);
//     void calculatePaginatedData();

//     QMap<QString, QJsonObject> m_shipData;
//     QVector<QString> m_shipOrder; // New member to store the order of ships
//     bool m_isLoading = false;
//     int m_currentPage = 1;
//     int m_itemsPerPage = 10;
//     QJsonArray m_paginatedData;
// };

// #endif // SHIPDATAMODEL_H

//********************************************************************************************************************************

// #ifndef SHIPDATAMODEL_H
// #define SHIPDATAMODEL_H

// #include <QObject>
// #include <QMap>
// #include <QJsonObject>

// class ShipDataModel : public QObject
// {
//     Q_OBJECT
//     Q_PROPERTY(QString shipData READ getShipData NOTIFY shipDataChanged)
//     Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)

// public:
//     explicit ShipDataModel(QObject *parent = nullptr);

//     Q_INVOKABLE void fetchShipData();
//     void addShip(const QString &uuid, const QJsonObject &shipData);
//     QString getShipData() const;
//     bool isLoading() const { return m_isLoading; }

// signals:
//     void shipDataChanged();
//     void isLoadingChanged();

// private:
//     QMap<QString, QJsonObject> m_shipData;
//     bool m_isLoading = false;
// };

// #endif // SHIPDATAMODEL_H

