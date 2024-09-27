#include "ShapefileManager.h"
#include "EncConvertor.h"
#include <QDirIterator>
#include <QDebug>
#include <QtConcurrent>

ShapefileManager::ShapefileManager(QObject *parent)
    : QObject(parent)
{
    m_threadPool.setMaxThreadCount(QThread::idealThreadCount());
}

void ShapefileManager::processFolders(const QStringList& folderPaths)
{
    m_isProcessing = true;

    // Delete outdated files before processing
    deleteOutdatedFiles(folderPaths);

    for (const QString& folderPath : folderPaths)
    {
        processFolder(folderPath);
    }
    m_isProcessing = false;
}

void ShapefileManager::processFolder(const QString& folderPath)
{
    m_isProcessing = true;
    QDir dir(folderPath);
    if (!dir.exists())
    {
        qDebug() << "Folder does not exist:" << folderPath;
        return;
    }

    QDirIterator it(folderPath, QStringList() << "*.000", QDir::Files, QDirIterator::Subdirectories);
    QList<QString> filePaths;

    while (it.hasNext())
    {
        filePaths.append(it.next());
    }

    // Process all files in this folder
    for (const QString& filePath : filePaths)
    {
        processFile(filePath);
    }
    m_isProcessing = false;
}

void ShapefileManager::processFile(const QString& filePath)
{
    QMutexLocker locker(&m_mutex);
    
    qDebug() << "Processing file:" << filePath;

    m_encConvertor.loadENC(filePath);
}

#include <QFileInfo>



void ShapefileManager::deleteOutdatedFiles(const QStringList& currentMapPaths)
{
    QSet<QString> currentInputFiles;
    QString outputFolder = QDir::currentPath() + "/ENC Shapefiles/";

    // Collect all current input files
    for (const QString& folderPath : currentMapPaths)
    {
        QDirIterator it(folderPath, QStringList() << "*.000", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext())
        {
            QString inputFile = it.next();
            QFileInfo fileInfo(inputFile);
            currentInputFiles.insert(fileInfo.completeBaseName());
        }
    }

    // Check and delete outdated output folders
    QDirIterator outputIt(outputFolder, QDir::Dirs | QDir::NoDotAndDotDot);
    while (outputIt.hasNext())
    {
        QDir dir(outputIt.next());
        QString folderName = dir.dirName();

        // If the corresponding .000 file doesn't exist in the current input files, delete the entire folder
        if (!currentInputFiles.contains(folderName))
        {
            if (dir.removeRecursively())
            {
                qDebug() << "Deleted outdated folder:" << dir.absolutePath();
            }
            else
            {
                qWarning() << "Failed to delete outdated folder:" << dir.absolutePath();
            }
        }
    }
}