#ifndef GUIMAPNIKVIEWER_H
#define GUIMAPNIKVIEWER_H

#include <QMainWindow>
#include <QListWidgetItem>

class SystemListWidget : public  QListWidgetItem {
private:
    DM::System * sys;
public:
    DM::System * getSystem() {return this->sys;}
    SystemListWidget(QString t, DM::System * sys): QListWidgetItem(t), sys(sys) {}
};

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

    void on_listWidget_systems_itemActivated(QListWidgetItem * item);
    void on_listWidget_systems_itemSelectionChanged();

    void addNewLayer(QString name);
    void addNewStyle(QString layername, QString nameOfStyle);
    void removeStyle(QString layername, QString nameOfStyle);
    void updatePredecessors();

signals:
    void changedSystem(DM::System * sys);

};

#endif // GUIMAPNIKVIEWER_H
