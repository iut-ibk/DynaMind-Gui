#ifndef MAPNIKSTYLEREADER_H
#define MAPNIKSTYLEREADER_H

#include <QXmlSimpleReader>

#include "guimapnikview.h"

class MapnikStyleReader : public QXmlDefaultHandler
{
private:
    GUIMapnikView * mv;
    QMap<QString, style_struct> styles;

    QString currentLayer;
    QString currentStyleName;

    style_struct current_style;

    QString tempval;
public:
    MapnikStyleReader(QString FileName, GUIMapnikView * mv);
    virtual ~MapnikStyleReader(){}
    bool startElement(const QString & namespaceURI,
                      const QString & localName,
                      const QString & qName,
                      const QXmlAttributes & atts);

    bool endElement(const QString & namespaceURI,
                    const QString & localName,
                    const QString & qName);

    bool characters(const QString &ch);

    bool fatalError(const QXmlParseException & exception);


};

#endif // MAPNIKSTYLEREADER_H
