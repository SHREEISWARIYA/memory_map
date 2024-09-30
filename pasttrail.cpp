#include "pastTrail.h"
#include <QUrlQuery>
#include <QGeoCoordinate>
#include <cmath>
#include <QVector>
#include <queue>

struct Triangle {
    int index;
    double area;
    Triangle(int i, double a) : index(i), area(a) {}
};

struct CompareTriangle {
    bool operator()(const Triangle& t1, const Triangle& t2) {
        return t1.area > t2.area;
    }
};

double calculateTriangleArea(const QGeoCoordinate& p1, const QGeoCoordinate& p2, const QGeoCoordinate& p3) {
    double a = p1.distanceTo(p2);
    double b = p2.distanceTo(p3);
    double c = p3.distanceTo(p1);
    double s = (a + b + c) / 2.0;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

QVector<QGeoCoordinate> visvalingamWhyatt(const QVector<QGeoCoordinate>& points, int targetPointCount) {
    if (points.size() <= targetPointCount) {
        return points;
    }

    QVector<QGeoCoordinate> simplifiedPoints = points;
    QVector<bool> removed(points.size(), false);
    std::priority_queue<Triangle, std::vector<Triangle>, CompareTriangle> pq;

    // Calculate initial areas
    for (int i = 1; i < simplifiedPoints.size() - 1; ++i) {
        double area = calculateTriangleArea(simplifiedPoints[i-1], simplifiedPoints[i], simplifiedPoints[i+1]);
        pq.push(Triangle(i, area));
    }

    int remainingPoints = simplifiedPoints.size();

    // Simplify until we reach the target point count
    while (remainingPoints > targetPointCount && !pq.empty()) {
        int indexToRemove = pq.top().index;
        pq.pop();

        if (!removed[indexToRemove]) {
            removed[indexToRemove] = true;
            remainingPoints--;

            // Recalculate areas for adjacent points
            for (int offset = -1; offset <= 1; offset += 2) {
                int i = indexToRemove + offset;
                if (i > 0 && i < simplifiedPoints.size() - 1 && !removed[i]) {
                    int prev = i - 1;
                    while (prev >= 0 && removed[prev]) prev--;
                    int next = i + 1;
                    while (next < simplifiedPoints.size() && removed[next]) next++;

                    if (prev >= 0 && next < simplifiedPoints.size()) {
                        double area = calculateTriangleArea(simplifiedPoints[prev], simplifiedPoints[i], simplifiedPoints[next]);
                        pq.push(Triangle(i, area));
                    }
                }
            }
        }
    }

    // Create the final simplified vector
    QVector<QGeoCoordinate> result;
    for (int i = 0; i < simplifiedPoints.size(); ++i) {
        if (!removed[i]) {
            result.append(simplifiedPoints[i]);
        }
    }

    return result;
}

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
    QUrl url("http://192.168.1.12:3000/api/ships/track-history/" + uuid);
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
