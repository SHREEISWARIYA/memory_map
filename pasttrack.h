// pastTrack.h
#ifndef PASTTRACK_H
#define PASTTRACK_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QMap>
#include <QVariantMap>

class PastTrack : public QObject
{
    Q_OBJECT
public:
    explicit PastTrack(QObject *parent = nullptr);
    Q_INVOKABLE void fetchTrackHistory(const QString &uuid, int hours);

signals:
    void trackHistoryFetched(const QVariantMap &trackHistory);

public slots:
    void onTrackHistoryFetched(QNetworkReply *reply);

private:
    QNetworkAccessManager *networkManager;
    QMap<QString, QVariantMap> memoryMap; // Use QMap to store data
    void printTrackHistory(const QVariantMap &trackHistory); // Helper function to print track history
};

#endif // PASTTRACK_H
