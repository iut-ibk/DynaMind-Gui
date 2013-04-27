#ifndef GUIMAPNIKVIEWER_H
#define GUIMAPNIKVIEWER_H

#include <QMainWindow>

namespace DM {
    class System;
}

namespace Ui {
class GUIMapnikViewer;
}

class GUIMapnikViewer : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GUIMapnikViewer(QWidget *parent = 0, DM::System * sys = 0);
    ~GUIMapnikViewer();
    
private:
    Ui::GUIMapnikViewer *ui;
    DM::System * sys;
};

#endif // GUIMAPNIKVIEWER_H
