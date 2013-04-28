#ifndef GUIMAPNIKVIEW_H
#define GUIMAPNIKVIEW_H

#include <QWidget>

namespace DM {
    class System;
}

namespace mapnik {
    class Map;
}
namespace Ui {
class GUIMapnikView;
}

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
    void addLayer(QString layer);
    
private:
    Ui::GUIMapnikView *ui;
    QPixmap pix_;
    mapnik::Map * map_;
    DM::System * sys_;

protected:
    void paintEvent(QPaintEvent* ev);
    void resizeEvent ( QResizeEvent * event );

};

#endif // GUIMAPNIKVIEW_H
