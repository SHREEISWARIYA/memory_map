#include "flagimg.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

FlagImg::FlagImg(QObject *parent) : QObject(parent) {}

QString FlagImg::getCountryCode(const QString &mmsi)
{
    qDebug() << "getCountryCode called with MMSI:" << mmsi;

    if (mmsi.isEmpty() || mmsi.length() < 3) {
        qDebug() << "Invalid MMSI: too short or empty";
        return "Unknown";
    }

    QString midsCode = mmsi.left(3);
    qDebug() << "Extracted MID code:" << midsCode;

    QString filePath = "C:/qtp/mem_pagination/CountryName_MMSI_CountryCode.csv";
    QFile file(filePath);

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qWarning() << "Failed to open CSV file:" << filePath;
        return "Unknown";
    }

    qDebug() << "CSV file opened successfully";


    QTextStream in(&file);
    int lineNumber = 0;
    while (!in.atEnd()) {
        QString line = in.readLine();
        lineNumber++;
        QStringList fields = line.split(",");

        if (fields.size() >= 3) {
            qDebug() << "Line" << lineNumber << "- MID:" << fields[0].trimmed() << "Country Code:" << fields[2].trimmed();

            if (fields[0].trimmed() == midsCode) {
                QString countryCode = fields[2].trimmed();
                qDebug() << "Match found! Returning country code:" << countryCode;
                file.close();
                return countryCode;
            }
        } else {
            qDebug() << "Line" << lineNumber << "has fewer than 3 fields:" << line;
        }
    }

    qDebug() << "No match found for MID code:" << midsCode;
    file.close();
    return "Unknown";
}
