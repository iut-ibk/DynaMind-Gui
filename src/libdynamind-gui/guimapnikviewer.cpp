#include "guimapnikviewer.h"
#include "ui_guimapnikviewer.h"
#include "guiviewselector.h"

#include "dmsystem.h"

GUIMapnikViewer::GUIMapnikViewer(QWidget *parent, DM::System * sys) :
    QMainWindow(parent),
    sys(sys),
    ui(new Ui::GUIMapnikViewer)
{
    ui->setupUi(this);
    ui->widget_mapnik->setSystem(sys);
}

GUIMapnikViewer::~GUIMapnikViewer()
{
    delete ui;
}

void GUIMapnikViewer::on_actionAdd_Layer_triggered()
{
    GUIViewSelector *vs = new GUIViewSelector(sys, this);
    vs->show();
    connect(vs, SIGNAL(selected(QString)), ui->widget_mapnik, SLOT(addLayer(QString)));

}
