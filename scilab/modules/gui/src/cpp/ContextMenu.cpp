/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#include "ContextMenu.hxx"

using namespace org_scilab_modules_gui_bridge;

void createContextMenu(char **menuLabels, int nbLabels)
{
  CallScilabBridge::newContextMenu(getScilabJavaVM(), menuLabels, nbLabels);
}
