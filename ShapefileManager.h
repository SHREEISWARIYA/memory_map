#ifndef SHAPEFILEMANAGER_H
#define SHAPEFILEMANAGER_H

#include <QMutexLocker>
#include <QObject>
#include <QStringList>
#include <QThreadPool>
#include "EncConvertor.h"

class ShapefileManager : public QObject
{
    Q_OBJECT

public:
    explicit ShapefileManager(QObject *parent = nullptr);
    void processFolders(const QStringList& folderPaths);
    bool isProcessing() const { return m_isProcessing; }

private:
    void processFolder(const QString& folderPath);
    void processFile(const QString& filePath);
    void deleteOutdatedFiles(const QStringList& currentMapPaths);

    
    QThreadPool m_threadPool;
    EncConvertor m_encConvertor;
    QMutex m_mutex;
    bool m_isProcessing;
};

#endif // SHAPEFILEMANAGER_H
