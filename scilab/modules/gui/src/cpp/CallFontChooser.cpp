/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
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
    return booltoBOOL(CallScilabBridge::getFontChooserBold(getScilabJavaVM(), fontChooserID));
}

BOOL getFontChooserItalic(int fontChooserID)
{
    return booltoBOOL(CallScilabBridge::getFontChooserItalic(getScilabJavaVM(), fontChooserID));
}
