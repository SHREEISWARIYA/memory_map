#ifndef SHIPTABLEMODEL_H
#define SHIPTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QMap>
#include <QJsonObject>
// Add these new includes
#include <QNetworkAccessManager>
#include <QNetworkReply>
// Add these new includes
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

// // Forward declaration of MessageType
// class MessageType;

class ShipTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit ShipTableModel(QObject *parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    Q_INVOKABLE QJsonObject getShipDataForRow(int row) const;

    void setShipData(const QMap<QString, QJsonObject> &shipData, const QVector<QString> &shipOrder);
    void setPage(int page, int itemsPerPage);
    void fetchMessageTypes();
    QString getMessageTypeDescription(int messageTypeId) const;
    QString getTrackNavStatus(int statusId) const;
    void fetchTrackNavStatuses();


signals:
    void messageTypesLoaded();
    void trackNavStatusesLoaded();

private slots:
    void onNetworkReply(QNetworkReply *reply);



private:
    QMap<QString, QJsonObject> m_shipData;
    QVector<QString> m_shipOrder;
    QVector<QString> m_headers;
    int m_currentPage = 1;
    int m_itemsPerPage = 10;
    static QString decimalToDMS(double decimal, bool isLatitude);
    QString formatTimestamp(const QVariant &timestamp) const;

    QNetworkAccessManager *m_networkManager;
    QMap<int, QVariantMap> m_messageTypeMap;
    QMap<int, QString> m_trackNavStatusMap;
};

#endif // SHIPTABLEMODEL_H
