#ifndef GUIRULEDEFINITION_H
#define GUIRULEDEFINITION_H

#include <QDialog>
#include <QColor>
#include <guimapnikview.h>

namespace Ui {
class GUIRuleDefinition;
}

class GUIRuleDefinition : public QDialog
{
    Q_OBJECT
    
public:
    explicit GUIRuleDefinition(QStringList attribute_list, QWidget *parent = 0);
    ~GUIRuleDefinition();

protected slots:
    void on_toolButton_color_clicked();
    void on_pushButton_add_attribute_clicked();
    void updateColorWidget(QColor c);
    void accept();
    
private:
    Ui::GUIRuleDefinition *ui;
    QColor current_color;


signals:
    void color_changed(QColor c);
    void newStyle(style_struct ss);

};

#endif // GUIRULEDEFINITION_H
