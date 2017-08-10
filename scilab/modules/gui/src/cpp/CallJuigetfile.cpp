/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - DIGITEO - Sylvestre KOUMAR
 * Copyright (C) 2009 - DIGITEO - Vincent COUVERT
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


#include "CallJuigetfile.hxx"
#include "Juigetfile.hxx"
#include "BOOL.h"

using namespace org_scilab_modules_gui_filechooser;

void CallJuigetfile(char ** mask, char ** description, int maskSize, char * initialDirectory, char * titleBox, bool multipleSelection)
{
    if (description == NULL)
    {
        Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, NULL, 0, initialDirectory, titleBox, multipleSelection);
    }
    else
    {
        Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, description, maskSize, initialDirectory, titleBox, multipleSelection);
    }
}

void CallJuigetfileWithoutInput()
{
    Juigetfile::uigetfile(getScilabJavaVM());
}

void CallJuigetfileOnlyWithMask(char ** mask, char ** description, int maskSize)
{
    if (description == NULL)
    {
        Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, NULL, 0);
    }
    else
    {
        Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, description, maskSize);
    }
}

void CallJuigetfileWithMaskAndInitialdirectory(char ** mask, char ** description, int maskSize, char * initialDirectory)
{
    if (description == NULL)
    {
        Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, NULL, 0, initialDirectory);
    }
    else
    {
        Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, description, maskSize, initialDirectory);
    }
}

void CallJuigetfileWithoutMultipleSelection(char ** mask, char ** description, int maskSize, char * initialDirectory, char * titleBox)
{
    if (description == NULL)
    {
        Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, NULL, 0, initialDirectory, titleBox);
    }
    else
    {
        Juigetfile::uigetfile(getScilabJavaVM(), mask, maskSize, description, maskSize, initialDirectory, titleBox);
    }
}

void CallJuiputfileWithoutInput()
{
    Juigetfile::uiputfile(getScilabJavaVM());
}

void CallJuiputfileOnlyWithMask(char ** mask, char ** description, int maskSize)
{
    if (description == NULL)
    {
        Juigetfile::uiputfile(getScilabJavaVM(), mask, maskSize, NULL, 0);
    }
    else
    {
        Juigetfile::uiputfile(getScilabJavaVM(), mask, maskSize, description, maskSize);
    }
}

void CallJuiputfileWithMaskAndInitialdirectory(char ** mask, char ** description, int maskSize, char * initialDirectory)
{
    if (description == NULL)
    {
        Juigetfile::uiputfile(getScilabJavaVM(), mask, maskSize, NULL, 0, initialDirectory);
    }
    else
    {
        Juigetfile::uiputfile(getScilabJavaVM(), mask, maskSize, description, maskSize, initialDirectory);
    }
}

void CallJuiputfileWithoutMultipleSelection(char ** mask, char ** description, int maskSize, char * initialDirectory, char * titleBox)
{
    if (description == NULL)
    {
        Juigetfile::uiputfile(getScilabJavaVM(), mask, maskSize, NULL, 0, initialDirectory, titleBox);
    }
    else
    {
        Juigetfile::uiputfile(getScilabJavaVM(), mask, maskSize, description, maskSize, initialDirectory, titleBox);
    }
}

char * getJuigetfileTitleBox()
{
    return Juigetfile::getTitleBox(getScilabJavaVM());
}

char ** getJuigetfileMask()
{
    return Juigetfile::getMask(getScilabJavaVM());
}

char * getJuigetfileInitialDirectory()
{
    return Juigetfile::getInitialDirectory(getScilabJavaVM());
}

int getJuigetfileSelectionSize()
{
    return Juigetfile::getSelectionSize(getScilabJavaVM());
}

char ** getJuigetfileSelection()
{
    return Juigetfile::getSelection(getScilabJavaVM());
}

char ** getJuigetfileSelectionFileNames()
{
    return Juigetfile::getSelectionFileNames(getScilabJavaVM());
}

bool getJuigetfileMultipleSelection()
{
    return Juigetfile::isMultipleSelection(getScilabJavaVM());
}

char * getJuigetfileSelectionPathName()
{
    return Juigetfile::getSelectionPathName(getScilabJavaVM());
}

int getJuigetfileFilterIndex()
{
    return Juigetfile::getFilterIndex(getScilabJavaVM());
}

char * getJuigetfileMenuCallback()
{
    return Juigetfile::getMenuCallback(getScilabJavaVM());
}

void deleteMenuCallBack(char * menuCallBack)
{
    delete[] menuCallBack;
}

void CallJuigetfileForDirectoryWithoutInput()
{
    Juigetfile::uigetdir(getScilabJavaVM());
}

void CallJuigetfileForDirectoryWithInitialdirectory(char *initialDirectory)
{
    Juigetfile::uigetdir(getScilabJavaVM(), initialDirectory);
}

void CallJuigetfileForDirectoryWithInitialdirectoryAndTitle(char *initialDirectory, char *title)
{
    Juigetfile::uigetdir(getScilabJavaVM(), initialDirectory, title);
}
