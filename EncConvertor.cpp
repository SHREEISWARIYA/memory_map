#include "EncConvertor.h"

void suppressGDALWarnings() {
    CPLSetErrorHandler([](CPLErr eErrClass, int err_no, const char *msg) {
        if (eErrClass == CE_Warning) {
            // Suppress warnings
            return;
        }
        // For errors and other cases, log as usual
        qDebug() << "GDAL : " << msg;
    });
}

EncConvertor::EncConvertor() : m_dataset(nullptr) {
    m_logFile.setFileName("enc_conversion_log.txt");
    if (m_logFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        m_outStream.setDevice(&m_logFile);
    } else {
        qDebug() << "Failed to open log file";
    }
    suppressGDALWarnings();
}

EncConvertor::~EncConvertor(){
   if (m_dataset)
    {
        GDALClose(m_dataset);
        m_dataset = nullptr;
    }
    if (m_logFile.isOpen()) {
        m_logFile.close();
    }
}


void EncConvertor::loadENC(const QString &filename){
    QFileInfo fileInfo(filename);
    QString baseName = fileInfo.baseName();
    QString outputFolder = QDir::currentPath() + "/ENC Shapefiles/" + baseName;
    QString metadataFilePath = outputFolder + "/metadata.json";

    if (QDir(outputFolder).exists())
    {
        if (QFile::exists(metadataFilePath))
        {
            m_outStream << "Shapefiles and metadata for " << filename << " already exist. Skipping...\n";
            return;
        }
        else
        {
            m_outStream << "Output folder exists but metadata file is missing. Recreating all files...\n";
            // Empty the folder
            QDir dir(outputFolder);
            dir.removeRecursively();
            dir.mkpath(".");
        }
    }
    else
    {
        QDir().mkpath(outputFolder);
    }

    // Load color map from CSV if not already loaded
    if (m_availableColors.isEmpty())
    {
        createChartColorMap(":/color_mst.csv");
    }

    // Rest of your loadENC function...
    m_outStream << "GDAL version: " << GDAL_RELEASE_NAME << "\n";

    qDebug() << "PROJ_LIB: " << CPLGetConfigOption("PROJ_LIB", "not set") << "\n";
    qDebug() << "GDAL_DATA: " << CPLGetConfigOption("GDAL_DATA", "not set") << "\n";

    m_outStream << "Available drivers:\n";
    for (int i = 0; i < GDALGetDriverCount(); ++i)
    {
        GDALDriverH driver = GDALGetDriver(i);
        m_outStream << "  " << GDALGetDriverShortName(driver) << "\n";
    }

    m_outStream << "Current working directory: " << QDir::currentPath() << "\n";

    m_outStream << "Attempting to open file: " << filename << "\n";

    // Check if file exists
    if (!fileInfo.exists())
    {
        m_outStream << "Error: File " << filename << " does not exist.\n";
        return;
    }

    m_outStream << "File size: " << fileInfo.size() << " bytes\n";

    GDALAllRegister();

    CPLSetConfigOption("OGR_S57_OPTIONS",
                       "RETURN_PRIMITIVES=ON,RETURN_LINKAGES=ON,LNAM_REFS=ON,SPLIT_MULTIPOINT=ON,"
                       "ADD_SOUNDG_DEPTH=ON");
    CPLSetConfigOption("CPL_DEBUG", "ON");
    CPLSetConfigOption("PROJ_LIB", "C:/OSGeo4W/share/proj/");

    m_dataset = GDALOpenEx(filename.toUtf8().constData(),
                           GDAL_OF_VECTOR,
                           nullptr,
                           nullptr,
                           nullptr);
    if (m_dataset == nullptr)
    {
        m_outStream << "Error opening file: " << filename << "\n";
        m_outStream << "GDAL error message: " << CPLGetLastErrorMsg() << "\n";
        return;
    }

    m_outStream << "File opened successfully\n";

    qDebug() << "Chart name:" << getChartNameFromTXT(filename);

    int numLayers = GDALDatasetGetLayerCount(m_dataset);
    qDebug() << "Number of layers:" << numLayers;

    OGRLayerH dsidLayer = nullptr;
    for (int i = 0; i < numLayers; ++i)
    {
        OGRLayerH layer = GDALDatasetGetLayer(m_dataset, i);
        if (layer == nullptr)
        {
            qDebug() << "Error getting layer:" << i;
            continue;
        }

        const char *layerName = OGR_L_GetName(layer);
        if (strcmp(layerName, "DSID") == 0)
        {
            dsidLayer = layer;
            break;
        }
    }

    if (dsidLayer == nullptr)
    {
        m_outStream << "Error: DSID layer not found.\n";
        GDALClose(m_dataset);
        return;
    }

    // Create metadata file
    createMetadataFile(filename, outputFolder, dsidLayer);

    // Create separate folders for Point, LineString, and Polygon geometries
    QString pointFolder = outputFolder + "/Point";
    QString lineStringFolder = outputFolder + "/LineString";
    QString polygonFolder = outputFolder + "/Polygon";
    QDir().mkpath(pointFolder);
    QDir().mkpath(lineStringFolder);
    QDir().mkpath(polygonFolder);

    createShapefiles(outputFolder);

    GDALClose(m_dataset);
}


void EncConvertor::analyzeLayer(int layerIndex, OGREnvelope &boundingBox){
    if (!m_dataset) return;

    OGRLayerH layer = GDALDatasetGetLayer(m_dataset, layerIndex);
    if (layer == nullptr) {
        m_outStream << "Error getting layer: " << layerIndex << "\n";
        return;
    }

    const char *layerName = OGR_L_GetName(layer);
    int featureCount = OGR_L_GetFeatureCount(layer, TRUE);
    m_outStream << "\nAnalyzing layer " << layerIndex << ": " << layerName << "\n";
    qDebug() << "\nAnalyzing layer " << layerIndex << ": " << layerName << "\n";
    m_outStream << "Total features: " << featureCount << "\n";
    qDebug() << "Total features: " << featureCount << "\n";

    OGRFeatureDefnH layerDefn = OGR_L_GetLayerDefn(layer);
    int fieldCount = OGR_FD_GetFieldCount(layerDefn);

    m_outStream << "Fields in this layer:\n";
    for (int i = 0; i < fieldCount; i++) {
        OGRFieldDefnH fieldDefn = OGR_FD_GetFieldDefn(layerDefn, i);
        m_outStream << "  " << OGR_Fld_GetNameRef(fieldDefn) << "\n";
    }

    OGR_L_ResetReading(layer);
    OGRFeatureH feature;
    int analyzedFeatures = 0;

    while ((feature = OGR_L_GetNextFeature(layer)) != nullptr) {
        analyzedFeatures++;
        m_outStream << "\n  Feature " << analyzedFeatures << ":\n";

        // Get geometry ---------------------------------------------------------------
        OGRGeometryH geometry = OGR_F_GetGeometryRef(feature);
        if (geometry != nullptr) {
            m_outStream << "    Geometry Type: " << OGR_G_GetGeometryName(geometry) << "\n";

            // Get WKT representation
            char *wkt = nullptr;
            OGRErr wktErr = OGR_G_ExportToWkt(geometry, &wkt);
            if (wktErr == OGRERR_NONE) {
                m_outStream << "    WKT: " << wkt << "\n";
                CPLFree(wkt);
            } else {
                m_outStream << "    Error getting WKT: " << CPLGetLastErrorMsg() << "\n";
            }

            //Bounding Box of this file ------------------------------------------
            OGREnvelope geometryEnvelope;
            OGR_G_GetEnvelope(geometry, &geometryEnvelope);

            // Update the bounding box with each feature's envelope
            if (boundingBox.IsInit()) {
                boundingBox.Merge(geometryEnvelope);
            } else {
                boundingBox = geometryEnvelope;
            }

        } else {
            m_outStream << "    No geometry\n";
        }

        // Get all attributes
        m_outStream << "    Attributes:\n";
        for (int i = 0; i < fieldCount; i++) {
            OGRFieldDefnH fieldDefn = OGR_FD_GetFieldDefn(layerDefn, i);
            const char *fieldName = OGR_Fld_GetNameRef(fieldDefn);
            if (OGR_F_IsFieldSet(feature, i)) {
                const char *fieldValue = OGR_F_GetFieldAsString(feature, i);
                m_outStream << "      " << fieldName << ": " << fieldValue << "\n";
            } else {
                m_outStream << "      " << fieldName << ": <not set>\n";
            }
        }

        OGR_F_Destroy(feature);
    }

    m_outStream << "Bounding Box for layer " << layerName << ": ";
    m_outStream << "MinX: " << boundingBox.MinX << ", MinY: " << boundingBox.MinY
              << ", MaxX: " << boundingBox.MaxX << ", MaxY: " << boundingBox.MaxY << "\n";

    m_outStream << "Total features analyzed: " << analyzedFeatures << "\n";
}

void EncConvertor::calculateBoundingBoxAndCenter(OGREnvelope &boundingBox){
    if (!m_dataset) return;

    int numLayers = GDALDatasetGetLayerCount(m_dataset);

    for (int i = 0; i < numLayers; ++i) {
        analyzeLayer(i, boundingBox);
    }

    double centerX = (boundingBox.MinX + boundingBox.MaxX) / 2.0;
    double centerY = (boundingBox.MinY + boundingBox.MaxY) / 2.0;

    m_outStream << "Center point: X = " << centerX << ", Y = " << centerY << "\n";

    double width = boundingBox.MaxX - boundingBox.MinX;
    double height = boundingBox.MaxY - boundingBox.MinY;
    double scale = std::max(width, height);

    m_outStream << "Zoom scale: " << scale << "\n";
}


void EncConvertor::createShapefiles(const QString &outputFolder) {
    if (!m_dataset) return;

    GDALDriverH shpDriver = GDALGetDriverByName("ESRI Shapefile");
    if (shpDriver == nullptr) {
        m_outStream << "ESRI Shapefile driver not available.\n";
        return;
    }

    int numLayers = GDALDatasetGetLayerCount(m_dataset);
    for (int i = 0; i < numLayers; ++i) {
        OGRLayerH layer = GDALDatasetGetLayer(m_dataset, i);
        if (layer == nullptr) {
            m_outStream << "Error getting layer " << i << "\n";
            continue;
        }

        const char *layerName = OGR_L_GetName(layer);

        // Check for the presence of each geometry type before creating shapefiles
        if (hasGeometryType(layer, wkbPoint)) {
            createGeometryShapefile(layer, layerName, wkbPoint, outputFolder + "/Point");
        }
        if (hasGeometryType(layer, wkbLineString)) {
            createGeometryShapefile(layer, layerName, wkbLineString, outputFolder + "/LineString");
        }
        if (hasGeometryType(layer, wkbPolygon)) {
            createGeometryShapefile(layer, layerName, wkbPolygon, outputFolder + "/Polygon");
        }
    }
}

void EncConvertor::createGeometryShapefile(OGRLayerH layer, const char *layerName, OGRwkbGeometryType geomType, const QString &folder) {
    GDALDriverH shpDriver = GDALGetDriverByName("ESRI Shapefile");
    if (shpDriver == nullptr) {
        m_outStream << "ESRI Shapefile driver not available.\n";
        return;
    }

    QString shapefilePath = QDir(folder).filePath(QString(layerName) + ".shp");
    m_outStream << "Creating Shapefile: " << shapefilePath << "\n";

    GDALDatasetH shpDataset = GDALCreate(shpDriver, shapefilePath.toUtf8().constData(), 0, 0, 0, GDT_Unknown, nullptr);
    if (shpDataset == nullptr) {
        m_outStream << "Error creating Shapefile: " << shapefilePath << "\n";
        return;
    }

    OGRLayerH shpLayer = GDALDatasetCreateLayer(shpDataset, layerName, OGR_L_GetSpatialRef(layer), geomType, nullptr);
    if (shpLayer == nullptr) {
        m_outStream << "Error creating layer in Shapefile: " << shapefilePath << "\n";
        GDALClose(shpDataset);
        return;
    }

    OGRFeatureDefnH layerDefn = OGR_L_GetLayerDefn(layer);
    int fieldCount = OGR_FD_GetFieldCount(layerDefn);

    // Add fields to the Shapefile layer
    for (int j = 0; j < fieldCount; ++j) {
        OGRFieldDefnH fieldDefn = OGR_FD_GetFieldDefn(layerDefn, j);
        OGRFieldType fieldType = OGR_Fld_GetType(fieldDefn);

        if (fieldType == OFTStringList || fieldType == OFTIntegerList || fieldType == OFTRealList) {
            // Convert list fields to string fields
            OGRFieldDefnH newFieldDefn = OGR_Fld_Create(OGR_Fld_GetNameRef(fieldDefn), OFTString);
            OGR_Fld_SetWidth(newFieldDefn, 129);  // Max width for DBF
            OGR_L_CreateField(shpLayer, newFieldDefn, TRUE);
            OGR_Fld_Destroy(newFieldDefn);
        } else {
            OGR_L_CreateField(shpLayer, fieldDefn, TRUE);
        }
    }

    // Copy features from the ENC layer to the Shapefile layer
    OGR_L_ResetReading(layer);
    OGRFeatureH feature;
    while ((feature = OGR_L_GetNextFeature(layer)) != nullptr) {
        OGRGeometryH geom = OGR_F_GetGeometryRef(feature);
        if (geom != nullptr && wkbFlatten(OGR_G_GetGeometryType(geom)) == wkbFlatten(geomType)) {
            OGRFeatureH shpFeature = OGR_F_Create(OGR_L_GetLayerDefn(shpLayer));
            OGR_F_SetFrom(shpFeature, feature, TRUE);
            OGR_L_CreateFeature(shpLayer, shpFeature);
            OGR_F_Destroy(shpFeature);
        }
        OGR_F_Destroy(feature);
    }

    GDALClose(shpDataset);
    m_outStream << "Shapefile created successfully: " << shapefilePath << "\n";
}

bool EncConvertor::hasGeometryType(OGRLayerH layer, OGRwkbGeometryType geomType) {
    OGR_L_ResetReading(layer);
    OGRFeatureH feature;
    while ((feature = OGR_L_GetNextFeature(layer)) != nullptr) {
        OGRGeometryH geom = OGR_F_GetGeometryRef(feature);
        if (geom != nullptr && wkbFlatten(OGR_G_GetGeometryType(geom)) == wkbFlatten(geomType)) {
            OGR_F_Destroy(feature);
            return true;
        }
        OGR_F_Destroy(feature);
    }
    return false;
}


void EncConvertor::createMetadataFile(const QString &filename, const QString &outputFolder, OGRLayerH dsidLayer){
    loadExistingMetadata(outputFolder);

    QJsonObject metadata;

    OGRFeatureH feature = OGR_L_GetNextFeature(dsidLayer);
    if (feature == nullptr)
    {
        m_outStream << "Error: No feature found in DSID layer.\n";
        return;
    }

    // Define the field names and their descriptions
    QMap<QString, QString> fieldDescriptions = {
        {"DSID_DSNM", "Data Set Name"},
        {"DSID_EDTN", "Edition Number"},
        {"DSID_UPDN", "Update Number"},
        {"DSID_UADT", "Update Application Date"},
        {"DSID_ISDT", "Issue Date"},
        {"DSPM_HDAT", "Horizontal Datum"},
        {"DSPM_VDAT", "Vertical Datum"},
        {"DSPM_SDAT", "Sounding Datum"},
        {"DSPM_CSCL", "Compilation Scale"},
        {"DSPM_DUNI", "Depth Units"},
        {"DSPM_HUNI", "Height Units"},
        {"DSPM_PUNI", "Positional Units"},
        {"DSPM_COUN", "Coordinate Units"},
        {"DSPM_COMF", "Compilation Scale Factor"},
        {"DSPM_SOMF", "Sounding Scale Factor"},
        {"DSPM_COMT", "Comment"}
    };

    QFileInfo fileInfo(filename);
    QString directoryPath = fileInfo.absolutePath();
    metadata["File Path"] = directoryPath;
    metadata["Chart Name"] = getChartNameFromTXT(filename);

    for (auto it = fieldDescriptions.begin(); it != fieldDescriptions.end(); ++it)
    {
        const QString &fieldName = it.key();
        const QString &description = it.value();
        int fieldIndex = OGR_F_GetFieldIndex(feature, fieldName.toUtf8().constData());

        if (fieldIndex == -1)
        {
            metadata[description] = "Field not found";
            continue;
        }

        OGRFieldType fieldType = OGR_Fld_GetType(OGR_F_GetFieldDefnRef(feature, fieldIndex));

        if (fieldName == "DSPM_HDAT" || fieldName == "DSPM_VDAT" || fieldName == "DSPM_SDAT")
        {
            int value = OGR_F_GetFieldAsInteger(feature, fieldIndex);
            QString datum = "Unknown";
            if (fieldName == "DSPM_HDAT" && value == 2) datum = "WGS84";
            if (fieldName == "DSPM_VDAT" && value == 16) datum = "WGS84";
            if (fieldName == "DSPM_SDAT" && value == 12) datum = "WGS84";
            metadata[description] = QString("%1 (%2)").arg(value).arg(datum);
        }
        else if (fieldName == "DSPM_HUNI" || fieldName == "DSPM_DUNI" || fieldName == "DSPM_PUNI" || fieldName == "DSPM_COUN"){
            int value = OGR_F_GetFieldAsInteger(feature, fieldIndex);
            if(value == 1)
                metadata[description] = "Meters";
            else
                 metadata[description] = value;
        }
        else
        {
            switch (fieldType)
            {
                case OFTInteger:
                    metadata[description] = OGR_F_GetFieldAsInteger(feature, fieldIndex);
                    break;
                case OFTReal:
                    metadata[description] = OGR_F_GetFieldAsDouble(feature, fieldIndex);
                    break;
                case OFTString:
                    metadata[description] = QString(OGR_F_GetFieldAsString(feature, fieldIndex));
                    break;
                case OFTDate:
                {
                    int year, month, day, hour, minute, second, tzFlag;
                    OGR_F_GetFieldAsDateTime(feature, fieldIndex, &year, &month, &day, &hour, &minute, &second, &tzFlag);
                    metadata[description] = QString("%1-%2-%3 %4:%5:%6").arg(year).arg(month, 2, 10, QChar('0'))
                                                           .arg(day, 2, 10, QChar('0')).arg(hour, 2, 10, QChar('0'))
                                                           .arg(minute, 2, 10, QChar('0')).arg(second, 2, 10, QChar('0'));
                    break;
                }
                default:
                    metadata[description] = "Unsupported field type";
            }
        }
    }

    OGR_F_Destroy(feature);

    // Add bounding color to metadata
    QString chartName = getChartNameFromTXT(filename);
    QColor chartColor;
    if (m_chartColors.contains(chartName))
    {
        chartColor = m_chartColors[chartName];
    }
    else
    {
        chartColor = getRandomChartColor();
        m_chartColors[chartName] = chartColor;
    }
    metadata["Bounding-Color"] = chartColor.name();

    QJsonDocument doc(metadata);
    QString jsonString = doc.toJson(QJsonDocument::Indented);

    QFile metadataFile(outputFolder + "/metadata.json");
    if (!metadataFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        m_outStream << "Error: Unable to create metadata file.\n";
        return;
    }

    QTextStream out(&metadataFile);
    out << jsonString;
    metadataFile.close();

    m_outStream << "Metadata file created successfully: " << outputFolder + "/metadata.json\n";
}

QString EncConvertor::getChartNameFromTXT(const QString &fullPath){
    QFileInfo fileInfo(fullPath);
    QString targetName = fileInfo.baseName();
    QString dirName = fileInfo.absolutePath();

    QDir dir(dirName);
    QStringList fileList = dir.entryList(QStringList() << "*.txt", QDir::Files);

    bool foundName = false;
    QString name;

    for (const QString &fileName : fileList) {
        QFile file(dir.absoluteFilePath(fileName));
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            qDebug() << "Error reading ENC .TXT file:" << file.fileName();
            continue;
        }

        QTextStream in(&file);
        QString line;
        while (!in.atEnd()) {
            line = in.readLine();
            if (line.startsWith(targetName, Qt::CaseInsensitive)) {
                QString tname = line.section('-', 1).trimmed();
                name = tname.trimmed();
                foundName = true;
                break;
            }
        }

        file.close();

        if (foundName) {
            break;
        }
    }

    if (!foundName) {
        qDebug() << "Chart name not found in any .TXT file.";
    }

    return name;
}



void EncConvertor::createChartColorMap(const QString &csvFilePath){
    QFile file(csvFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error: Could not open color CSV file.";
        return;
    }

    QTextStream in(&file);
    // Skip header
    in.readLine();

    while (!in.atEnd())
    {
        QString line = in.readLine();
        QStringList fields = line.split(",");
        if (fields.size() >= 7)
        {
            QColor color(fields[4].toInt(), fields[5].toInt(), fields[6].toInt());
            m_availableColors.append(color);
        }
    }

    file.close();
}

QColor EncConvertor::getRandomChartColor(){
    if (m_availableColors.isEmpty())
    {
        qDebug() << "Error: No colors available in the color map.";
        return QColor();
    }

    int index = QRandomGenerator::global()->bounded(m_availableColors.size());
    QColor color = m_availableColors.takeAt(index);
    return color;
}

void EncConvertor::loadExistingMetadata(const QString &outputFolder){
    QString metadataFilePath = outputFolder + "/metadata.json";
    QFile metadataFile(metadataFilePath);
    if (!metadataFile.exists())
    {
        return;
    }

    if (!metadataFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "Error: Could not open existing metadata file.";
        return;
    }

    QByteArray jsonData = metadataFile.readAll();
    metadataFile.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(jsonData, &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        qDebug() << "Error: Failed to parse metadata JSON.";
        return;
    }

    QJsonObject metadata = doc.object();
    QString chartName = metadata["Chart Name"].toString();
    QString boundingColor = metadata["Bounding-Color"].toString();

    if (!boundingColor.isEmpty())
    {
        m_chartColors[chartName] = QColor(boundingColor);
    }
}



