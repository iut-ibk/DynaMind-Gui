#include "guistyledefinition.h"
#include "ui_guistyledefinition.h"
#include "guiruledefinition.h"

GUIStyleDefinition::GUIStyleDefinition(QString layername, QStringList stylenames, QStringList attribute_list, QWidget *parent) :
    QDialog(parent),
    layerName(layername),
    attribute_list(attribute_list),
    ui(new Ui::GUIStyleDefinition)
{
    ui->setupUi(this);
    ui->listWidget_rules->addItems(stylenames);
}

GUIStyleDefinition::~GUIStyleDefinition()
{
    delete ui;
}

void GUIStyleDefinition::on_pushButton_remove_style_clicked()
{
    if (!ui->listWidget_rules->currentItem()) return;
    emit removeStyle(layerName, ui->listWidget_rules->currentItem()->text());
    delete ui->listWidget_rules->currentItem();
}

void GUIStyleDefinition::on_pushButton_add_style_clicked()
{
    GUIRuleDefinition * grd = new GUIRuleDefinition(attribute_list, this);
    connect(grd, SIGNAL(newStyle(style_struct)), this, SLOT(new_style_structed_created(style_struct)));
    grd->show();
}

void GUIStyleDefinition::new_style_structed_created(style_struct ss)
{
    ss.layer = this->layerName;
    ui->listWidget_rules->addItem(ss.name);
    emit newStyle(ss);
}
