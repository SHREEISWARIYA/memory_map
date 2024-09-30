#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QMap>

class MessageType : public QObject
{
    Q_OBJECT

public:
    explicit MessageType(QObject *parent = nullptr);
    void fetchMessageTypes();

signals:
    void messageTypesLoaded();

private slots:
    void onNetworkReply(QNetworkReply *reply);

private:
    QNetworkAccessManager *manager;
    QMap<int, QVariantMap> messageTypeMap;
};

#endif // MESSAGETYPE_H
