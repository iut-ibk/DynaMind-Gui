#include "guiruledefinition.h"
#include "ui_guiruledefinition.h"
#include <QColorDialog>

GUIRuleDefinition::GUIRuleDefinition(QStringList attribute_list, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GUIRuleDefinition)
{
    ui->setupUi(this);



    QStringList symbolizer;
    symbolizer << "LineSymbolizer"
               << "PolygonSymbolizer"
               << "BuildingSymbolizer";

    ui->comboBox_symbolizer->addItems(symbolizer);
    ui->listWidget_attributes->addItems(attribute_list);
    QColor c(Qt::black);
    this->updateColorWidget(c);

}

GUIRuleDefinition::~GUIRuleDefinition()
{
    delete ui;
}

void GUIRuleDefinition::on_toolButton_color_clicked()
{
    QColor color = QColorDialog::getColor(this->current_color, this);
    this->updateColorWidget(color);


}

void GUIRuleDefinition::on_pushButton_add_attribute_clicked()
{
    if(!ui->listWidget_attributes->currentItem()) return;

    if(ui->listWidget_attributes->currentItem()->text().isEmpty()) return;

    QString varaible = ui->lineEdit_filter->text() + " [" + ui->listWidget_attributes->currentItem()->text() + "]";
    ui->lineEdit_filter->setText(varaible);
}


void GUIRuleDefinition::updateColorWidget(QColor c)
{
    current_color = c;
    QPalette pal(ui->frame_color->palette());
    pal.setColor(QPalette::Background, c);
    ui->frame_color->setPalette(c);
    ui->frame_color->setAutoFillBackground(true);
}

void GUIRuleDefinition::accept()
{
    style_struct ss;
    ss.name = ui->lineEdit_name->text();
    ss.symbolizer = ui->comboBox_symbolizer->currentText();
    ss.buildingHeight = ui->lineEdit_building_height->text();
    ss.color = this->current_color;
    ss.linewidth = ui->doubleSpinBox_width->value();
    ss.filter = ui->lineEdit_filter->text();
    ss.opacity = ui->doubleSpinBox->value();

    emit newStyle(ss);

    QDialog::accept();
}
