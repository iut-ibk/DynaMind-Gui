#ifndef GUISTYLEDEFINITION_H
#define GUISTYLEDEFINITION_H

#include <guimapnikview.h>
#include <QDialog>

namespace Ui {
class GUIStyleDefinition;
}

class GUIStyleDefinition : public QDialog
{
    Q_OBJECT
    
public:
    explicit GUIStyleDefinition(QString layername, QStringList styleNames,QStringList attribute_list, QWidget *parent = 0);
    ~GUIStyleDefinition();

protected slots:
    void on_pushButton_remove_style_clicked();
    void on_pushButton_add_style_clicked();
    void new_style_structed_created(style_struct ss);
    
private:
    Ui::GUIStyleDefinition *ui;
    QString layerName;
    QStringList attribute_list;

signals:
    void removeStyle(QString layername, QString style);
    void newStyle(style_struct ss);

};

#endif // GUISTYLEDEFINITION_H
