/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
#include "uimenu.hxx"
//#include "ScilabGraphicWindow.hxx" // TODO move path to Makefile
#include "DrawableFigure.h"
#include "DrawableFigureBridge.h"
#include "DrawableFigureJoGL.h"
/*-----------------------------------------------------------------------------------*/
//using namespace org_scilab_modules_gui_bridge_menu;
using namespace org_scilab_modules_gui_graphicWindow;
using namespace sciGraphics;
void uimenu(void)
{
  JavaVM scilabJVM = *getScilabJavaVM();

  // Scilab figure
  //DrawableFigureJoGL::openRenderingCanvas(1);

  // Scilab Main View Toolbar

  // Creation of the menu
  std::cout << "allo ?" << std::endl;
  //SwingScilabMenu *SSM = new SwingScilabMenu(&scilabJVM);
  std::cout << "allo ?" << std::endl;
  //(*SSM).setText("pouet");
  //(*SSM).setCallback("disp(1)");
  std::cout << "allo ?" << std::endl;
}
/*-----------------------------------------------------------------------------------*/
