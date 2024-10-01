#ifndef FLAGIMG_H
#define FLAGIMG_H

#include <QObject>

class FlagImg : public QObject
{
    Q_OBJECT
public:
    explicit FlagImg(QObject *parent = nullptr);

    Q_INVOKABLE QString getCountryCode(const QString &mmsi);
};

#endif // FLAGIMG_H
