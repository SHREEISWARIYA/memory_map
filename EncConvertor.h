#ifndef ENCCONVERTOR_H
#define ENCCONVERTOR_H

#include <QDebug>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonParseError>
#include <QMap>
#include <QColor>
#include <QRandomGenerator>

#include "gdal_priv.h"
#include "ogrsf_frmts.h"

class EncConvertor
{
public:
    EncConvertor();
    ~EncConvertor();

    void loadENC(const QString &filename);

    void analyzeLayer(int layerIndex, OGREnvelope &boundingBox);
    void calculateBoundingBoxAndCenter(OGREnvelope &boundingBox);

    void createShapefiles(const QString &outputFolder);
    void createGeometryShapefile(OGRLayerH layer, const char *layerName, OGRwkbGeometryType geomType, const QString &folder);
    bool hasGeometryType(OGRLayerH layer, OGRwkbGeometryType geomType);

    void createMetadataFile(const QString &filename, const QString &outputFolder, OGRLayerH dsidLayer);
    QString getChartNameFromTXT(const QString &fullPath);

    // New function to create memory map of chart colors
    void createChartColorMap(const QString &csvFilePath);
    
    // New function to get a random color for a chart
    QColor getRandomChartColor();

    // New function to load existing metadata and populate memory map
    void loadExistingMetadata(const QString &outputFolder);

private:
    QMap<QString, QColor> m_chartColors;
    QVector<QColor> m_availableColors;

    GDALDatasetH m_dataset;
    QFile m_logFile;
    QTextStream m_outStream;
};

#endif // ENCCONVERTOR_H
