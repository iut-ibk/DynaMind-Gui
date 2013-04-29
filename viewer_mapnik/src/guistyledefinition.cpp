#include "guistyledefinition.h"
#include "ui_guistyledefinition.h"

GUIStyleDefinition::GUIStyleDefinition(QString layername, QStringList stylenames, QWidget *parent) :
    QDialog(parent),
    layerName(layername),
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
