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

public slots:
    //Toolbar
    void on_actionAdd_Layer_triggered();
    void on_actionEdit_Styles_triggered();
    void on_actionSave_to_picture_triggered();
    void on_actionSaveStyle_triggered();
    void on_actionLoad_style_triggered();

    void addNewLayer(QString name);
    void addNewStyle(QString layername, QString nameOfStyle);
    void removeStyle(QString layername, QString nameOfStyle);

};

#endif // GUIMAPNIKVIEWER_H
