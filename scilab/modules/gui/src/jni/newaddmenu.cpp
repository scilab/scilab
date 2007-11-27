/*-----------------------------------------------------------------------------------*/
/**
* @author Vincent COUVERT - INRIA 2007
*/
/*-----------------------------------------------------------------------------------*/
//#include "newaddmenu.hxx"
/*-----------------------------------------------------------------------------------*/
using namespace  org_scilab_modules_gui_bridge_menu;
void newaddmenu(char *label)
{
  JavaVM scilabJVM = *getScilabJavaVM();

  SwingScilabMenu *jMenu = new SwingScilabMenu(&scilabJVM);
  (*jMenu).setText(label);
}
/*-----------------------------------------------------------------------------------*/

