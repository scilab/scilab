/* Copyright INRIA */
/* Vincent COUVERT 2007 */

#include "CallMessageBox.hxx"

using namespace org_scilab_modules_gui_bridge;

int createMessageBox()
{
  return CallScilabBridge::newMessageBox(getScilabJavaVM());
}

void setMessageBoxTitle(int ID, char * title)
{
  CallScilabBridge::setMessageBoxTitle(getScilabJavaVM(), ID, title);
}

void setMessageBoxMessage(int ID, char * message)
{
  CallScilabBridge::setMessageBoxMessage(getScilabJavaVM(), ID, message);
}

void setMessageBoxMultiLineMessage(int ID, char **message, int nbLines)
{
  CallScilabBridge::setMessageBoxMessage(getScilabJavaVM(), ID, message, nbLines);
}

void messageBoxDisplayAndWait(int ID)
{
  CallScilabBridge::messageBoxDisplayAndWait(getScilabJavaVM(), ID);
}

int getMessageBoxSelectedButton(int ID)
{
  return CallScilabBridge::getMessageBoxSelectedButton(getScilabJavaVM(), ID);
}

void setMessageBoxButtonsLabels(int ID, char** labels, int nbLabels)
{
  CallScilabBridge::setMessageBoxButtonsLabels(getScilabJavaVM(), ID, labels, nbLabels);
}

void setMessageBoxInitialValue(int ID, char** value, int nbLines)
{
  CallScilabBridge::setMessageBoxInitialValue(getScilabJavaVM(), ID, value, nbLines);
}

char ** getMessageBoxValue(int ID)
{
  return CallScilabBridge::getMessageBoxValue(getScilabJavaVM(), ID);
}

int getMessageBoxValueSize(int ID)
{
  return CallScilabBridge::getMessageBoxValueSize(getScilabJavaVM(), ID);
}

void setMessageBoxListBoxItems(int ID, char** items, int nbItems)
{
  CallScilabBridge::setMessageBoxListBoxItems(getScilabJavaVM(), ID, items, nbItems);
}

int getMessageBoxSelectedItem(int ID)
{
  return CallScilabBridge::getMessageBoxSelectedItem(getScilabJavaVM(), ID);
}

void setMessageBoxLineLabels(int ID, char** labels, int nbLabels)
{
  CallScilabBridge::setMessageBoxLineLabels(getScilabJavaVM(), ID, labels, nbLabels);
}
	
void setMessageBoxColumnLabels(int ID, char** labels, int nbLabels)
{
  CallScilabBridge::setMessageBoxColumnLabels(getScilabJavaVM(), ID, labels, nbLabels);
}
	
void setMessageBoxDefaultInput(int ID, char** values, int nbValues)
{
  CallScilabBridge::setMessageBoxDefaultInput(getScilabJavaVM(), ID, values, nbValues);
}


