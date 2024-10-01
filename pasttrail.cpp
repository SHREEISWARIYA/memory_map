#include "pastTrail.h"
#include <QUrlQuery>
#include <QGeoCoordinate>
#include <cmath>
#include <QVector>
#include <queue>

double perpendicularDistance(const QGeoCoordinate& point, const QGeoCoordinate& lineStart, const QGeoCoordinate& lineEnd) {
    double x = point.longitude();
    double y = point.latitude();
    double x1 = lineStart.longitude();
    double y1 = lineStart.latitude();
    double x2 = lineEnd.longitude();
    double y2 = lineEnd.latitude();

    double A = x - x1;
    double B = y - y1;
    double C = x2 - x1;
    double D = y2 - y1;

    double dot = A * C + B * D;
    double len_sq = C * C + D * D;
    double param = dot / len_sq;

    double xx, yy;

    if (param < 0) {
        xx = x1;
        yy = y1;
    }
    else if (param > 1) {
        xx = x2;
        yy = y2;
    }
    else {
        xx = x1 + param * C;
        yy = y1 + param * D;
    }

    QGeoCoordinate projectedPoint(yy, xx);
    return point.distanceTo(projectedPoint);
}

void simplifyLine(const QVector<QGeoCoordinate>& points, double epsilon, QVector<QGeoCoordinate>& result, int start, int end) {
    if (end - start < 2) {
        return;
    }

    double maxDistance = 0;
    int indexFarthest = start;

    for (int i = start + 1; i < end; i++) {
        double distance = perpendicularDistance(points[i], points[start], points[end]);
        if (distance > maxDistance) {
            maxDistance = distance;
            indexFarthest = i;
        }
    }

    if (maxDistance > epsilon) {
        simplifyLine(points, epsilon, result, start, indexFarthest);
        result.append(points[indexFarthest]);
        simplifyLine(points, epsilon, result, indexFarthest, end);
    }
}

QVector<QGeoCoordinate> optimizeLine(const QVector<QGeoCoordinate>& points, double epsilon) {
    if (points.size() < 3) {
        return points;
    }

    QVector<QGeoCoordinate> result;
    result.append(points.first());
    simplifyLine(points, epsilon, result, 0, points.size() - 1);
    result.append(points.last());

    return result;
}

PastTrail::PastTrail(QObject *parent) : QObject(parent)
{
    m_manager = new QNetworkAccessManager(this);
    connect(m_manager, &QNetworkAccessManager::finished, this, &PastTrail::onNetworkReply);
}

void PastTrail::fetchTrackHistory(const QString &uuid, int hours)
{
    QUrl url("http://localhost:3000/api/ships/track-history/" + uuid);
    QUrlQuery query;
    qDebug() << " Fetching .............";
    query.addQueryItem("hours", QString::number(hours));
    url.setQuery(query);

    QNetworkRequest request(url);
    m_manager->get(request);
}

void PastTrail::onNetworkReply(QNetworkReply *reply)
{
    if (reply->error() == QNetworkReply::NoError) {
        QByteArray data = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(data);
        QJsonObject jsonObj = jsonDoc.object();

        QString uuid = jsonObj["uuid"].toString();
        QJsonArray trackHistoryArray = jsonObj["trackHistory"].toArray();

        QVector<QGeoCoordinate> points;
        QVector<QString> timestamps;
        for (const QJsonValue &value : trackHistoryArray) {
            QJsonObject point = value.toObject();
            points.append(QGeoCoordinate(point["latitude"].toDouble(), point["longitude"].toDouble()));
            timestamps.append(point["timestamp"].toString());
        }

        double epsilon = 45.0001; // Adjust this value to control the level of simplification
        QVector<QGeoCoordinate> simplifiedPoints = optimizeLine(points, epsilon);

        QVariantList trackHistory;
        for (const QGeoCoordinate &point : simplifiedPoints) {
            QVariantMap pointMap;
            pointMap["latitude"] = point.latitude();
            pointMap["longitude"] = point.longitude();
            int originalIndex = points.indexOf(point);
            pointMap["timestamp"] = timestamps[originalIndex];
            trackHistory.append(pointMap);
        }

        qDebug() << "------------------- > Emitting trackHistoryFetched signal for UUID:" << uuid
                 << "Original points:" << points.size() 
                 << "Simplified points:" << trackHistory.size();
        m_history[uuid] = trackHistory;
        emit trackHistoryFetched(uuid, trackHistory);
    } else {
        qDebug() << "Network error:" << reply->errorString();
    }

    reply->deleteLater();
}
