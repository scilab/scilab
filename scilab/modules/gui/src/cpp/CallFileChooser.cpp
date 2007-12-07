/* Copyright INRIA */
/* Vincent COUVERT 2007 */

#include "CallFileChooser.hxx"

using namespace org_scilab_modules_gui_bridge;

int createFileChooser()
{
  return CallScilabBridge::newFileChooser(getScilabJavaVM());
}

void setFileChooserTitle(int ID, char * title)
{
  CallScilabBridge::setFileChooserTitle(getScilabJavaVM(), ID, title);
}

void setFileChooserMask(int ID, char * mask)
{
  CallScilabBridge::setFileChooserMask(getScilabJavaVM(), ID, mask);
}

void setFileChooserInitialDirectory(int ID, char * path)
{
  CallScilabBridge::setFileChooserInitialDirectory(getScilabJavaVM(), ID, path);
}

void fileChooserDisplayAndWait(int ID)
{
  CallScilabBridge::fileChooserDisplayAndWait(getScilabJavaVM(), ID);
}

char** getFileChooserSelection(int ID)
{
  return CallScilabBridge::getFileChooserSelection(getScilabJavaVM(), ID);
}

int getFileChooserSelectionSize(int ID)
{
  return CallScilabBridge::getFileChooserSelectionSize(getScilabJavaVM(), ID);
}
