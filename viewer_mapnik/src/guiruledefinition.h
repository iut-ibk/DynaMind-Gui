#ifndef GUIRULEDEFINITION_H
#define GUIRULEDEFINITION_H

#include <QDialog>

namespace Ui {
class GUIRuleDefinition;
}

class GUIRuleDefinition : public QDialog
{
    Q_OBJECT
    
public:
    explicit GUIRuleDefinition(QWidget *parent = 0);
    ~GUIRuleDefinition();
    
private:
    Ui::GUIRuleDefinition *ui;
};

#endif // GUIRULEDEFINITION_H
