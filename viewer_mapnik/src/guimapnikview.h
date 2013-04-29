#ifndef GUIMAPNIKVIEW_H
#define GUIMAPNIKVIEW_H


//QT
#include <QWidget>

struct style_struct {
    QString layer;
    QString symbolizer;
    QString name;
    QString buildingHeight;
    QString filter;
    QColor color;
    double linewidth;
    double opacity;

};

namespace DM {
    class System;
}

namespace mapnik {
    class Map;
}
namespace Ui {
class GUIMapnikView;
}

struct mapnik_private;

class GUIMapnikView : public QWidget
{
    Q_OBJECT
    
public:
    explicit GUIMapnikView( QWidget *parent = 0, DM::System * sys=0);
    ~GUIMapnikView();

public slots:
    void init_mapnik();
    void drawMap();
    void setSystem(DM::System * sys);
    void addLayer(QString dm_layer);
    void editStyleDefintionGUI(QString layer);
    void removeStyleDefinition(QString layer_name, QString stylename);
    void addNewStyle(style_struct ss);
    void saveToPicture(unsigned width, unsigned height, QString filename);
    
private:
    Ui::GUIMapnikView *ui;
    QPixmap pix_;
    mapnik::Map * map_;
    DM::System * sys_;
    mapnik_private * d;

    /** return index of layer, if not found return value is -1*/
    int getLayerIndex(std::string layer_name);

protected:
    void paintEvent(QPaintEvent* ev);
    void resizeEvent ( QResizeEvent * event );

signals:
    void new_layer_added(QString);
    void new_style_added(QString layer_name, QString style_name);
    void removedStyle(QString layer_name, QString stylename);
};

#endif // GUIMAPNIKVIEW_H
