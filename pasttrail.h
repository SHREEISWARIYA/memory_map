#ifndef PASTTRAIL_H
#define PASTTRAIL_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>
#include <QVariant>

class PastTrail : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariantMap history READ history NOTIFY historyChanged)

public:
    explicit PastTrail(QObject *parent = nullptr);

    Q_INVOKABLE void fetchTrackHistory(const QString &uuid, int hours);
    QVariantMap history() const { return m_history; }

signals:
    void trackHistoryFetched(const QString &uuid, const QVariantList &trackHistory);
    void historyChanged();

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_manager;
    QVariantMap m_history;
};

#endif // PASTTRAIL_H
