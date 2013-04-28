#include "guiruledefinition.h"
#include "ui_guiruledefinition.h"

GUIRuleDefinition::GUIRuleDefinition(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GUIRuleDefinition)
{
    ui->setupUi(this);
}

GUIRuleDefinition::~GUIRuleDefinition()
{
    delete ui;
}
