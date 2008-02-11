/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#include "ContextMenu.hxx"

using namespace org_scilab_modules_gui_bridge;

char* createContextMenu(char **menuLabels, int nbLabels)
{
  return CallScilabBridge::newContextMenu(getScilabJavaVM(), menuLabels, nbLabels);
}
