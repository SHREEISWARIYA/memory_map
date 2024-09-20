#include "cursorcontroller.h"
#include <QCursor>
#include <QGuiApplication>

CursorController::CursorController(QObject *parent) : QObject(parent) {}

void CursorController::moveCursor(const QPoint &pos)
{
    QCursor::setPos(pos);
}
