#ifndef GUISTYLEDEFINITION_H
#define GUISTYLEDEFINITION_H

#include <QDialog>

namespace Ui {
class GUIStyleDefinition;
}

class GUIStyleDefinition : public QDialog
{
    Q_OBJECT
    
public:
    explicit GUIStyleDefinition(QString layername, QStringList styleNames, QWidget *parent = 0);
    ~GUIStyleDefinition();

protected slots:
    void on_pushButton_remove_style_clicked();
    
private:
    Ui::GUIStyleDefinition *ui;
    QString layerName;

signals:
    void removeStyle(QString layername, QString style);

};

#endif // GUISTYLEDEFINITION_H
