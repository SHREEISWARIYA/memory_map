#include "backgroundprocessor.h"

BackgroundTask::BackgroundTask(ShapefileManager *manager, const QStringList &paths)
    : m_manager(manager)
    , m_paths(paths)
{}

void BackgroundTask::run()
{
    m_manager->processFolders(m_paths);
}

BackgroundProcessor::BackgroundProcessor(QObject *parent)
    : QObject(parent), m_isProcessing(false)
{
    m_checkTimer.setInterval(1000);  // Check every second
    connect(&m_checkTimer, &QTimer::timeout, this, &BackgroundProcessor::checkProcessingStatus);
}

void BackgroundProcessor::startProcessing(UserSettings *userSettings)
{
    QStringList folderPaths = userSettings->mapPaths();
    BackgroundTask *task = new BackgroundTask(&m_shapefileManager, folderPaths);

    m_isProcessing = true;
    m_checkTimer.start();

    QThreadPool::globalInstance()->start(task);
}

void BackgroundProcessor::checkProcessingStatus()
{
    // You need to implement a method in ShapefileManager to check if processing is complete
    if (!m_shapefileManager.isProcessing())
    {
        m_isProcessing = false;
        m_checkTimer.stop();
        emit processingComplete();
    }
}
