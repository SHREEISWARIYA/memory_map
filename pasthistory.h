// pasthistory.h
#ifndef PASTHISTORY_H
#define PASTHISTORY_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMap>
#include <QVariant>

class PastHistory : public QObject
{
    Q_OBJECT

public:
    explicit PastHistory(QObject *parent = nullptr);

    Q_INVOKABLE void fetchTrackHistory(const QString &shipUuid, int hours);
    Q_INVOKABLE QVariantList getTrackHistory(const QString &shipUuid) const;

signals:
    void trackHistoryFetched(const QString &shipUuid);

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *m_networkManager;
    QMap<QString, QVariantList> m_trackHistoryMap;
};

#endif // PASTHISTORY_H
