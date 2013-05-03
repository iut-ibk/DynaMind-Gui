
#include "ui_guimapnikviewer.h"

#include "guimapnikviewer.h"
#include "guiviewselector.h"
#include "guisavefiletopng.h"
#include "mapnikstylereader.h"

#include <QFileDialog>

#include "dmsystem.h"
#include "dmlogger.h"

GUIMapnikViewer::GUIMapnikViewer(QWidget *parent, DM::System * sys) :
    QMainWindow(parent),
    sys(sys),
    ui(new Ui::GUIMapnikViewer)
{
    ui->setupUi(this);
    ui->widget_mapnik->setSystem(sys);
    ui->treeWidget->setColumnCount(1);

    connect(ui->widget_mapnik, SIGNAL(new_layer_added(QString)), this, SLOT(addNewLayer(QString)));
    connect(ui->widget_mapnik, SIGNAL(new_style_added(QString,QString)), this, SLOT(addNewStyle(QString,QString)));
    connect(ui->widget_mapnik, SIGNAL(removedStyle(QString,QString)), this, SLOT(removeStyle(QString,QString)));
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

void GUIMapnikViewer::on_actionSave_to_picture_triggered()
{

    GUISaveFileToPNG * gsf = new GUISaveFileToPNG(this);
    connect(gsf, SIGNAL(choosen_file_options(uint,uint,QString)), ui->widget_mapnik, SLOT(saveToPicture(uint,uint,QString)));
    gsf->show();
}

void GUIMapnikViewer::on_actionSaveStyle_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save Style File"));
    if (fileName.isEmpty()) {
        return;
    }
    if (!fileName.contains(".sty"))
        fileName+=".sty";

    QFile file(fileName);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << QString::fromStdString(ui->widget_mapnik->save_style_to_file());
    file.close();

}

void GUIMapnikViewer::on_actionLoad_style_triggered()
{


    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Style File"), "", tr("Style Files (*.sty)"));

    if (fileName.isEmpty())
        return;


    MapnikStyleReader mr(fileName, this->ui->widget_mapnik);
}


void GUIMapnikViewer::addNewLayer(QString name)
{
    QStringList items;
    items << name;
    QTreeWidgetItem * item = new QTreeWidgetItem((QTreeWidgetItem*)0, items);
    item->setExpanded(true);
    ui->treeWidget->addTopLevelItem(item);
}

void GUIMapnikViewer::addNewStyle(QString layername, QString nameOfStyle)
{
    QTreeWidgetItem * root_item = ui->treeWidget->findItems(layername, Qt::MatchExactly, 0)[0];
    QStringList items;
    items << nameOfStyle;
    QTreeWidgetItem * item = new QTreeWidgetItem(root_item, items);
}

void GUIMapnikViewer::removeStyle(QString layername, QString nameOfStyle)
{
    QTreeWidgetItem * root_item = ui->treeWidget->findItems(layername, Qt::MatchExactly, 0)[0];
    QTreeWidgetItem * item_delete = NULL;
    for (int i = 0; i < root_item->childCount(); i++) {
        if (root_item->child(i)->text(0) == nameOfStyle) item_delete = root_item->child(i);
    }

    if (item_delete) delete item_delete;
}
