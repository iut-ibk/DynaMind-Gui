#include "guimapnikviewer.h"
#include "ui_guimapnikviewer.h"

#include "dmsystem.h"

GUIMapnikViewer::GUIMapnikViewer(QWidget *parent, DM::System * sys) :
    QMainWindow(parent),
    sys(sys),
    ui(new Ui::GUIMapnikViewer)
{
    ui->setupUi(this);
}

GUIMapnikViewer::~GUIMapnikViewer()
{
    delete ui;
}
