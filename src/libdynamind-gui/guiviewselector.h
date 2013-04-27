#ifndef GUIVIEWSELECTOR_H
#define GUIVIEWSELECTOR_H

#include <QDialog>

namespace DM {
    class System;
}

namespace Ui {
class GUIViewSelector;
}

class GUIViewSelector : public QDialog
{
    Q_OBJECT
    
public:
    explicit GUIViewSelector(DM::System * sys, QWidget *parent = 0);
    ~GUIViewSelector();
    
private:
    Ui::GUIViewSelector *ui;

public slots:
    void accept();

signals:
    void selected(QString);

};

#endif // GUIVIEWSELECTOR_H
