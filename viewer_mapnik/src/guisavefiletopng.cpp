#include "guisavefiletopng.h"
#include "ui_guisavefiletopng.h"
#include <QFileDialog>

GUISaveFileToPNG::GUISaveFileToPNG(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GUISaveFileToPNG)
{
    ui->setupUi(this);
}

GUISaveFileToPNG::~GUISaveFileToPNG()
{
    delete ui;
}

void GUISaveFileToPNG::accept()
{
    if (!ui->lineEdit_filename->text().isEmpty()){
        emit choosen_file_options(ui->spinBox_width->value(), ui->spinBox_height->value(),ui->lineEdit_filename->text() );
    }

    QDialog::accept();
}

void GUISaveFileToPNG::on_toolButton_choose_file_clicked()
{
    QString filename = QFileDialog::getSaveFileName(this, "Save file", "", ".png");
    if (filename.isEmpty()) return;
    QString f = filename + ".png";
    ui->lineEdit_filename->setText(f);
}


