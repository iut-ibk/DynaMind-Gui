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
    explicit GUIMapnikView(DM::System * sys, QWidget *parent = 0);
    ~GUIMapnikView();
public slots:
    void init_mapnik();
    void drawMap();
    
private:
    Ui::GUIMapnikView *ui;
    QPixmap pix_;
    mapnik::Map * map_;
    DM::System * sys_;

protected:
    void paintEvent(QPaintEvent* ev);
};

#endif // GUIMAPNIKVIEW_H
