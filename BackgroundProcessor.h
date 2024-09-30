#ifndef BACKGROUNDPROCESSOR_H
#define BACKGROUNDPROCESSOR_H

#include <QObject>
#include <QThreadPool>
#include <QRunnable>
#include <QTimer>
#include "shapefilemanager.h"
#include "usersettings.h"

class BackgroundTask : public QRunnable
{
public:
    BackgroundTask(ShapefileManager *manager, const QStringList &paths);
    void run() override;

private:
    ShapefileManager *m_manager;
    QStringList m_paths;
};

class BackgroundProcessor : public QObject
{
    Q_OBJECT

public:
    explicit BackgroundProcessor(QObject *parent = nullptr);
    void startProcessing(UserSettings *userSettings);

signals:
    void processingComplete();

private slots:
    void checkProcessingStatus();

private:
    ShapefileManager m_shapefileManager;
    QTimer m_checkTimer;
    bool m_isProcessing;
};

#endif // BACKGROUNDPROCESSOR_H
