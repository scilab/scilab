/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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

void setFileChooserDirectorySelectionOnly(int ID)
{
  CallScilabBridge::setFileChooserDirectorySelectionOnly(getScilabJavaVM(), ID);
}

void setFileChooserFileSelectionOnly(int ID)
{
  CallScilabBridge::setFileChooserFileSelectionOnly(getScilabJavaVM(), ID);
}
