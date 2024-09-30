#include "usersettings.h"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QStandardPaths>
#include <QDir>
#include <QCoreApplication>
#include <QFileDialog>

UserSettings::UserSettings(QObject *parent) : QObject(parent)
{

    // Get the directory where the executable is located
       QString exeDir = QCoreApplication::applicationDirPath();

       // Create the settings file path in the same directory as the executable
       m_settingsFilePath = exeDir + QDir::separator() + "user.json";

       loadSettings();
}

QStringList UserSettings::mapPaths() const
{
    return m_mapPaths;
}

// void UserSettings::setMapPaths(const QStringList &paths)
// {
//     if (m_mapPaths != paths) {
//         m_mapPaths = paths;
//         emit mapPathsChanged();
//     }
// }

bool UserSettings::saveSettings()
{
    QJsonObject json;
    QJsonArray pathsArray;
    for (const QString &path : m_mapPaths) {
        pathsArray.append(path);
    }
    json["map_paths"] = pathsArray;

    QJsonDocument doc(json);
    QFile file(m_settingsFilePath);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();
        qDebug() << "Settings saved successfully to" << m_settingsFilePath;
        return true;
    } else {
        qWarning() << "Failed to save settings to" << m_settingsFilePath;
        return false;
    }
}

void UserSettings::loadSettings()
{
    QFile file(m_settingsFilePath);
    
    if (!file.exists()) {
        // File doesn't exist, create a new one with default settings
        if (file.open(QIODevice::WriteOnly)) {
            QJsonObject json;
            json["map_paths"] = QJsonArray();
            QJsonDocument doc(json);
            file.write(doc.toJson());
            file.close();
            qDebug() << "Created new settings file:" << m_settingsFilePath;
        } else {
            qWarning() << "Failed to create settings file:" << m_settingsFilePath;
            return;
        }
    }

    if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        file.close();

        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject json = doc.object();
        QJsonArray pathsArray = json["map_paths"].toArray();

        m_mapPaths.clear();
        for (const QJsonValue &value : pathsArray) {
            m_mapPaths.append(value.toString());
        }
        emit mapPathsChanged();
    } else {
        qWarning() << "Failed to open settings file:" << m_settingsFilePath;
    }
}

QString UserSettings::addMapPath()
{
    QString dir = QFileDialog::getExistingDirectory(nullptr, 
        tr("Select Directory"),
        QDir::homePath(),
        QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        return dir;
    }
    return QString();  // Return an empty string if no directory was selected
}

void UserSettings::setMapPaths(const QStringList &paths)
{
    if (m_mapPaths != paths) {
        m_mapPaths = paths;
        emit mapPathsChanged();
        if (saveSettings()) {
            emit mapPathsUpdated();
        } else {
            qWarning() << "Failed to save updated map paths";
        }
    }
}
