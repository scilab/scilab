/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

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

void setMessageBoxDefaultSelectedButtons(int ID, int* indices, int nbIndices)
{
    CallScilabBridge::setMessageBoxDefaultSelectedButtons(getScilabJavaVM(), ID, indices, nbIndices);
}

int* getMessageBoxUserSelectedButtons(int ID)
{
    return CallScilabBridge::getMessageBoxUserSelectedButtons(getScilabJavaVM(), ID);
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

void setMessageBoxModal(int ID, BOOL status)
{
    CallScilabBridge::setMessageBoxModal(getScilabJavaVM(), ID, BOOLtobool(status));
}

void setMessageBoxIcon(int ID, char *name)
{
    CallScilabBridge::setMessageBoxIcon(getScilabJavaVM(), ID, name);
}



