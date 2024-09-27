#ifndef USERSETTINGS_H
#define USERSETTINGS_H

#include <QObject>
#include <QStringList>

class UserSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList mapPaths READ mapPaths WRITE setMapPaths NOTIFY mapPathsChanged)

public:
    explicit UserSettings(QObject *parent = nullptr);

    QStringList mapPaths() const;


    Q_INVOKABLE bool saveSettings();
    Q_INVOKABLE void loadSettings();

public slots:
    QString addMapPath();
    void setMapPaths(const QStringList &paths);

signals:
    void mapPathsChanged();
    void mapPathsUpdated();  // New signal

private:
    QStringList m_mapPaths;
    QString m_settingsFilePath;
};

#endif // USERSETTINGS_H
