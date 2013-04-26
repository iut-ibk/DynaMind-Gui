#include "viewer2d_prototype.h"



#include <iostream>

#include <guimapnikview.h>


viewer2d_prototype::viewer2d_prototype(DM::System *sys)
{

    GUIMapnikView * view = new GUIMapnikView(sys);
    view->show();

}
