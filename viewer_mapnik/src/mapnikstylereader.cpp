#include "dmlogger.h"

#include "mapnikstylereader.h"



MapnikStyleReader::MapnikStyleReader(QString file, GUIMapnikView * mv) :
    mv(mv)
{

    QXmlSimpleReader r;
    if (!QFile::exists(file)) {
        return;
    }
    QFile f(file);
    r.setContentHandler(this);
    r.parse(QXmlInputSource(&f));
}

bool MapnikStyleReader::startElement(const QString &namespaceURI, const QString &localName, const QString &qName, const QXmlAttributes &atts)
{

    Q_UNUSED(namespaceURI)
    Q_UNUSED(localName)

    if (qName == "Map") {
        if (atts.index("srs") >= 0) {
            DM::Logger(DM::Debug) <<  atts.value("srs").toStdString();
        }
        return true;
    }
    if (qName == "Style") {
        current_style = style_struct();
        if (atts.index("name") >= 0) {
            DM::Logger(DM::Debug) <<  atts.value("name").toStdString();
            current_style.name =  atts.value("name");
            current_style.opacity = atts.value("opacity").toDouble();
        }
        return true;
    }


    if (qName == "Rule") {
        return true;
    }

    if (qName == "LineSymbolizer") {
        current_style.symbolizer = "LineSymbolizer";
        if (atts.index("stroke") >= 0) {
            DM::Logger(DM::Debug) <<  atts.value("stroke").toStdString();
            QString color_string = atts.value("stroke");
            color_string.replace("rgb(", "");
            color_string.replace(")", "");
            QStringList color_list = color_string.split(",", QString::SkipEmptyParts);
            DM::Logger(DM::Debug) << color_string.toStdString();
            current_style.color = QColor(color_list[0].toInt(),color_list[1].toInt(),color_list[2].toInt());

        }
        if (atts.index("stroke-width") >= 0) {
            DM::Logger(DM::Debug) <<  atts.value("stroke-width").toDouble();
            current_style.linewidth = atts.value("stroke-width").toDouble();
        }
        return true;
    }

    if (qName == "PolygonSymbolizer") {
        current_style.symbolizer =  "PolygonSymbolizer";
        if (atts.index("fill") >= 0) {
            DM::Logger(DM::Debug) <<  atts.value("fill").toStdString();
            QString color_string = atts.value("fill");
            color_string.replace("rgb(", "");
            color_string.replace(")", "");
            QStringList color_list = color_string.split(",", QString::SkipEmptyParts);
            DM::Logger(DM::Debug) << color_string.toStdString();
            current_style.color = QColor(color_list[0].toInt(),color_list[1].toInt(),color_list[2].toInt());

        }
        return true;
    }
    if (qName == "BuildingSymbolizer") {
        current_style.symbolizer =  "BuildingSymbolizer";
        if (atts.index("fill") >= 0) {
            DM::Logger(DM::Debug) <<  atts.value("fill").toStdString();
            QString color_string = atts.value("fill");
            color_string.replace("rgb(", "");
            color_string.replace(")", "");
            QStringList color_list = color_string.split(",", QString::SkipEmptyParts);
            DM::Logger(DM::Debug) << color_string.toStdString();
            current_style.color = QColor(color_list[0].toInt(),color_list[1].toInt(),color_list[2].toInt());

        }
        if (atts.index("height") >= 0) {
            DM::Logger(DM::Debug) <<  atts.value("height").toStdString();
            current_style.buildingHeight = atts.value("height");
        }
        return true;
    }
    if (qName == "Filter") {
        if (atts.index("value") >= 0) {
            QString filter = atts.value("value");
            filter.replace("&gt;", ">");
            filter.replace("&lt;", "<");
            current_style.filter = filter;

        }
        return true;
    }

    if (qName == "Layer") {
        if (atts.index("name") >= 0) {
            DM::Logger(DM::Debug) <<  "Add Layer " <<atts.value("name").toStdString();
            this->mv->addLayer(atts.value("name"), false);
            currentLayer = atts.value("name");

            DM::Logger(DM::Debug) << atts.count();
        }
        return true;
    }

    if (qName == "Stylename") {
        DM::Logger(DM::Debug) << "attscount " <<atts.count();
        this->tempval.clear();
        if (atts.index("style") >= 0) {
            currentStyleName = atts.value("style");
        }

        return true;


    }
    return true;
}

bool MapnikStyleReader::fatalError(const QXmlParseException &exception)
{
    DM::Logger(DM::Error) << "fatal error while parsing xml " << exception.message().toStdString();

    return true;
}

bool MapnikStyleReader::endElement(const QString & namespaceURI,
                                   const QString & localName,
                                   const QString & qName) {

    if (qName == "Style") {
        styles[current_style.name] = current_style;
        current_style = style_struct();
        return true;
    }

    if (qName == "Stylename") {
        currentStyleName = this->tempval;
        style_struct style_to_add = styles[currentStyleName];
        style_to_add.layer = currentLayer;
        DM::Logger(DM::Debug) << "Current Layer " << currentLayer.toStdString();
        mv->addNewStyle(style_to_add);
        return true;
    }
    return true;
}

bool MapnikStyleReader::characters(const QString &ch)
{
    this->tempval = ch;
    return true;
}
