#ifndef SHIPDATAMODEL_H
#define SHIPDATAMODEL_H

#include <QObject>
#include <QMap>
#include <QJsonObject>

class ShipDataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString shipData READ getShipData NOTIFY shipDataChanged)
    Q_PROPERTY(bool isLoading READ isLoading NOTIFY isLoadingChanged)

public:
    explicit ShipDataModel(QObject *parent = nullptr);

    Q_INVOKABLE void fetchShipData();
    void addShip(const QString &uuid, const QJsonObject &shipData);
    QString getShipData() const;
    bool isLoading() const { return m_isLoading; }

signals:
    void shipDataChanged();
    void isLoadingChanged();

private:
    QMap<QString, QJsonObject> m_shipData;
    bool m_isLoading = false;
};

#endif // SHIPDATAMODEL_H
