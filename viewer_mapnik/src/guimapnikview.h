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
    void increaseZoomLevel(double factor);
    void decreaseZoomLevel(double factor);


private:
    Ui::GUIMapnikView *ui;
    QPixmap pix_;
    mapnik::Map * map_;
    DM::System * sys_;
    mapnik_private * d;
    double zoom_level;
    QPoint pan;

    QPoint startPos;

    /** return index of layer, if not found return value is -1*/
    int getLayerIndex(std::string layer_name);

protected:
    void paintEvent(QPaintEvent* ev);
    void resizeEvent ( QResizeEvent * ev );
    void wheelEvent(QWheelEvent * ev);
    void mousePressEvent(QMouseEvent * ev);
    void mouseMoveEvent(QMouseEvent *ev);

signals:
    void new_layer_added(QString);
    void new_style_added(QString layer_name, QString style_name);
    void removedStyle(QString layer_name, QString stylename);
    void zoom_in(double factor);
    void zoom_out(double factor);

};

#endif // GUIMAPNIKVIEW_H
