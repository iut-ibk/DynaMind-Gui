#include "guiviewselector.h"
#include "ui_guiviewselector.h"

#include <dmsystem.h>

GUIViewSelector::GUIViewSelector(DM::System *sys, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GUIViewSelector)
{
    ui->setupUi(this);

    std::vector<DM::View> views = sys->getViews();

    QStringList view_names;
    foreach (DM::View v, views) {
        view_names << QString::fromStdString(v.getName());
    }

    ui->listWidget->addItems(view_names);
}

GUIViewSelector::~GUIViewSelector()
{
    delete ui;
}

void GUIViewSelector::accept()
{
    emit selected(ui->listWidget->currentItem()->text());

    QDialog::accept();
}


