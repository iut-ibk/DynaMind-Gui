#include "viewer2d_prototype.h"



#include <iostream>

#include <guimapnikviewer.h>


viewer2d_prototype::viewer2d_prototype(DM::System *sys)
{

    GUIMapnikViewer * viewer = new GUIMapnikViewer(0,sys);
    viewer->show();

}
