/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan CORNET
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

/*--------------------------------------------------------------------------*/
#include "XlFontManager.hxx"

extern "C" {
#include <stdlib.h>
#include "getScilabJavaVM.h"
#include "RendererFontManager.h"
#include "sci_malloc.h"
#include "os_string.h"
}

using namespace org_scilab_modules_renderer_utils_textRendering;

/*--------------------------------------------------------------------------*/
int getNbInstalledFonts(void)
{
    XlFontManager * fntmgr = new XlFontManager(getScilabJavaVM());
    int res = fntmgr->getSizeInstalledFontsName();
    delete fntmgr;
    return res;
}
/*--------------------------------------------------------------------------*/
char **getInstalledFontsName(int *sizeArray)
{
    char **returnedinstalledfontsname = NULL;
    *sizeArray = 0;

    org_scilab_modules_renderer_utils_textRendering::XlFontManager *fntmgr = new org_scilab_modules_renderer_utils_textRendering::XlFontManager(getScilabJavaVM());
    if (fntmgr)
    {
        char **installedfontsname = NULL;

        *sizeArray = fntmgr->getSizeInstalledFontsName();
        installedfontsname = fntmgr->getInstalledFontsName();
        if (installedfontsname)
        {
            int i = 0;
            returnedinstalledfontsname = (char**)MALLOC(sizeof(char*) * (*sizeArray));
            for ( i = 0; i < *sizeArray; i++)
            {
                returnedinstalledfontsname[i] = os_strdup(installedfontsname[i]);
                delete [] installedfontsname[i];
            }
            delete [] installedfontsname;
            installedfontsname = NULL;
        }
        delete fntmgr;
    }

    return returnedinstalledfontsname;
}
/*--------------------------------------------------------------------------*/
char **getAvailableFontsName(int *sizeArray)
{
    char **returnedavailablefontsname = NULL;
    *sizeArray = 0;

    org_scilab_modules_renderer_utils_textRendering::XlFontManager *fntmgr = new org_scilab_modules_renderer_utils_textRendering::XlFontManager(getScilabJavaVM());
    if (fntmgr)
    {
        char **availablefontsname = NULL;

        *sizeArray = fntmgr->getSizeAvailableFontsName();
        availablefontsname = fntmgr->getAvailableFontsName();
        if (availablefontsname)
        {
            int i = 0;
            returnedavailablefontsname = (char**)MALLOC(sizeof(char*) * (*sizeArray));
            for ( i = 0; i < *sizeArray; i++)
            {
                returnedavailablefontsname[i] = os_strdup(availablefontsname[i]);
                delete [] availablefontsname[i];
            }
            delete [] availablefontsname;
            availablefontsname = NULL;
        }
        delete fntmgr;
    }

    return returnedavailablefontsname;
}
/*--------------------------------------------------------------------------*/
BOOL isAvailableFontsName(char *fontname)
{
    BOOL bOK = FALSE;
    org_scilab_modules_renderer_utils_textRendering::XlFontManager *fntmgr = new org_scilab_modules_renderer_utils_textRendering::XlFontManager(getScilabJavaVM());
    if (fntmgr)
    {
        bOK = booltoBOOL(fntmgr->isAvailableFontName(fontname));
        delete fntmgr;
    }
    return bOK;
}
/*--------------------------------------------------------------------------*/
int addFont(char *fontname)
{
    int fontID = 0;

    org_scilab_modules_renderer_utils_textRendering::XlFontManager *fntmgr = new org_scilab_modules_renderer_utils_textRendering::XlFontManager(getScilabJavaVM());
    if (fntmgr)
    {
        fontID = (int)fntmgr->addFont(fontname);
        delete fntmgr;
    }

    return fontID;
}
/*--------------------------------------------------------------------------*/
int changeFont(int index, char * fontName)
{
    int fontID = 0;
    org_scilab_modules_renderer_utils_textRendering::XlFontManager *fntmgr = new org_scilab_modules_renderer_utils_textRendering::XlFontManager(getScilabJavaVM());
    if (fntmgr)
    {
        fontID = (int)fntmgr->changeFont((long)index, fontName);
        delete fntmgr;
    }

    return fontID;
}
/*--------------------------------------------------------------------------*/
int changeFontWithProperty(int index, char * fontName, BOOL isBold, BOOL isItalic)
{
    int fontID = 0;
    org_scilab_modules_renderer_utils_textRendering::XlFontManager *fntmgr = new org_scilab_modules_renderer_utils_textRendering::XlFontManager(getScilabJavaVM());
    if (fntmgr)
    {
        fontID = (int)fntmgr->changeFontWithProperty((long)index, fontName, BOOLtobool(isBold), BOOLtobool(isItalic));
        delete fntmgr;
    }
    return fontID;
}
/*--------------------------------------------------------------------------*/
void resetFontManager(void)
{
    org_scilab_modules_renderer_utils_textRendering::XlFontManager *fntmgr = new org_scilab_modules_renderer_utils_textRendering::XlFontManager(getScilabJavaVM());
    if (fntmgr)
    {
        fntmgr->resetXlFontManager();
        delete fntmgr;
    }
}
/*--------------------------------------------------------------------------*/
int changeFontFromFilename (int index, char * FontFilename)
{
    int fontID = 0;
    org_scilab_modules_renderer_utils_textRendering::XlFontManager *fntmgr = new org_scilab_modules_renderer_utils_textRendering::XlFontManager(getScilabJavaVM());
    if (fntmgr)
    {
        fontID = (int)fntmgr->changeFontFromFilename((long)index, FontFilename);
        delete fntmgr;
    }
    return fontID;
}
/*--------------------------------------------------------------------------*/
int addFontFromFilename (char * FontFilename)
{
    int fontID = 0;
    org_scilab_modules_renderer_utils_textRendering::XlFontManager *fntmgr = new org_scilab_modules_renderer_utils_textRendering::XlFontManager(getScilabJavaVM());
    if (fntmgr)
    {
        fontID = (int)fntmgr->addFontFromFilename (FontFilename);
        delete fntmgr;
    }
    return fontID;
}
/*--------------------------------------------------------------------------*/
