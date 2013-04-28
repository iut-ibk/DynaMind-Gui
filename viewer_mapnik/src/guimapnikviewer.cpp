
#include "ui_guimapnikviewer.h"

#include "guimapnikviewer.h"
#include "guiviewselector.h"


#include "dmsystem.h"

GUIMapnikViewer::GUIMapnikViewer(QWidget *parent, DM::System * sys) :
    QMainWindow(parent),
    sys(sys),
    ui(new Ui::GUIMapnikViewer)
{
    ui->setupUi(this);
    ui->widget_mapnik->setSystem(sys);
    ui->treeWidget->setColumnCount(2);

    connect(ui->widget_mapnik, SIGNAL(new_layer_added(QString)), this, SLOT(addNewLayer(QString)));
    connect(ui->widget_mapnik, SIGNAL(new_style_added(QString,QString)), this, SLOT(addNewStyle(QString,QString)));
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

void GUIMapnikViewer::on_actionEdit_Styles_triggered()
{
    QString layer (ui->treeWidget->currentItem()->text(0));
    ui->widget_mapnik->editStyleDefintionGUI(layer);
}

void GUIMapnikViewer::addNewLayer(QString name)
{
    QStringList items;
    items << name;
    QTreeWidgetItem * item = new QTreeWidgetItem((QTreeWidgetItem*)0, items);

    ui->treeWidget->addTopLevelItem(item);
}

void GUIMapnikViewer::addNewStyle(QString layername, QString nameOfStyle)
{
    QTreeWidgetItem * root_item = ui->treeWidget->findItems(layername, Qt::MatchExactly, 0)[0];
    QStringList items;
    items << nameOfStyle;
    QTreeWidgetItem * item = new QTreeWidgetItem(root_item, items);
}
