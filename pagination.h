#ifndef PAGINATION_H
#define PAGINATION_H

#include <QObject>
#include <QMap>
#include <QVariantMap>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class Pagination : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QMap<QString, QVariantMap> ships READ ships NOTIFY shipsChanged)

public:
    explicit Pagination(QObject *parent = nullptr);
    Q_INVOKABLE QMap<QString, QVariantMap> ships() const;
    Q_INVOKABLE void printShipDetails(const QString &uuid) const;

signals:
    void shipsChanged();

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager;
    QMap<QString, QVariantMap> m_pagination;
};

#endif // PAGINATION_H
