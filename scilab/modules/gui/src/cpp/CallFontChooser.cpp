/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CallFontChooser.hxx"

using namespace org_scilab_modules_gui_bridge;

int createFontChooser()
{
  return CallScilabBridge::newFontChooser(getScilabJavaVM());
}

void setFontChooserFontName(int fontChooserID, char *fontName)
{
  CallScilabBridge::setFontChooserFontName(getScilabJavaVM(), fontChooserID, fontName);
}

void setFontChooserFontSize(int fontChooserID, int fontSize)
{
  CallScilabBridge::setFontChooserFontSize(getScilabJavaVM(), fontChooserID, fontSize);
}

void setFontChooserBold(int fontChooserID, BOOL bold)
{
  CallScilabBridge::setFontChooserBold(getScilabJavaVM(), fontChooserID, BOOLtobool(bold));
}

void setFontChooserItalic(int fontChooserID, BOOL italic)
{
  CallScilabBridge::setFontChooserItalic(getScilabJavaVM(), fontChooserID, BOOLtobool(italic));
}

void fontChooserDisplayAndWait(int fontChooserID)
{
  CallScilabBridge::fontChooserDisplayAndWait(getScilabJavaVM(), fontChooserID);
}

char *getFontChooserFontName(int fontChooserID)
{
  return CallScilabBridge::getFontChooserFontName(getScilabJavaVM(), fontChooserID);
}

int getFontChooserFontSize(int fontChooserID)
{
  return CallScilabBridge::getFontChooserFontSize(getScilabJavaVM(), fontChooserID);
}

BOOL getFontChooserBold(int fontChooserID)
{
  return CallScilabBridge::getFontChooserBold(getScilabJavaVM(), fontChooserID);
}

BOOL getFontChooserItalic(int fontChooserID)
{
  return CallScilabBridge::getFontChooserItalic(getScilabJavaVM(), fontChooserID);
}
