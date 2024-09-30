#ifndef SHIPDATA_H
#define SHIPDATA_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QMap>
#include <QVariant>
#include <QVariantList>

class ShipData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int totalShips READ totalShips NOTIFY totalShipsChanged)
    Q_PROPERTY(QVariantList shipList READ shipList NOTIFY shipsChanged)
    Q_PROPERTY(QVariantList mmsiUuidList READ mmsiUuidList NOTIFY mmsiUuidListChanged)

public:
    explicit ShipData(QObject *parent = nullptr);

    Q_INVOKABLE void fetchShips();
    int totalShips() const;
    QVariantList shipList() const;
    QVariantList mmsiUuidList() const;
    Q_INVOKABLE QVariantMap isValidShip(const QString &uuid) const;
    Q_INVOKABLE QString getUuidFromMmsi(const QString &mmsi) const;
    Q_INVOKABLE QVariantMap getShipDetails(const QString &uuid) const;
    Q_INVOKABLE void printShipDetails(const QString &uuid) const;

signals:
    void totalShipsChanged();
    void shipsChanged();
    void mmsiUuidListChanged();

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_networkManager;
    QMap<QString, QVariantMap> m_ships;
    QMap<QString, QString> m_mmsiUuidMap;
    int m_totalShips;

};

#endif // SHIPDATA_H
