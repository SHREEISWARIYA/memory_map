#ifndef CURSORCONTROLLER_H
#define CURSORCONTROLLER_H

#include <QObject>
#include <QPoint>

class CursorController : public QObject
{
    Q_OBJECT
public:
    explicit CursorController(QObject *parent = nullptr);

public slots:
    void moveCursor(const QPoint &pos);
};

#endif // CURSORCONTROLLER_H
