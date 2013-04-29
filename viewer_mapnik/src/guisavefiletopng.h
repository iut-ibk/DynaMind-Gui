#ifndef GUISAVEFILETOPNG_H
#define GUISAVEFILETOPNG_H

#include <QDialog>

namespace Ui {
class GUISaveFileToPNG;
}

class GUISaveFileToPNG : public QDialog
{
    Q_OBJECT
    
public:
    explicit GUISaveFileToPNG(QWidget *parent = 0);
    ~GUISaveFileToPNG();

protected slots:
    void accept();

public slots:
    void on_toolButton_choose_file_clicked();
    
private:
    Ui::GUISaveFileToPNG *ui;

signals:
    void choosen_file_options(unsigned width, unsigned height, QString );
};

#endif // GUISAVEFILETOPNG_H
