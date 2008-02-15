/* Copyright INRIA 2008 */
/* Vincent COUVERT */

#include "ContextMenu.hxx"

using namespace org_scilab_modules_gui_bridge;

// char* createContextMenu(char **menuLabels, int nbLabels)
// {
//   return CallScilabBridge::newContextMenu(getScilabJavaVM(), menuLabels, nbLabels);
// }

void createContextMenu(sciPointObj* sciObj)
{
  pUICONTEXTMENU_FEATURE(sciObj)->hashMapIndex = CallScilabBridge::newContextMenu(getScilabJavaVM());
}

char* uiWaitContextMenu(sciPointObj* sciObj)
{
  return CallScilabBridge::displayAndWaitContextMenu(getScilabJavaVM(), pUICONTEXTMENU_FEATURE(sciObj)->hashMapIndex);
}

