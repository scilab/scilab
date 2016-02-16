/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
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
#include <string.h> // memcpy
#include "configgraphicvariable.hxx"

/*
** FPF
** \{
*/

std::wstring ConfigGraphicVariable::m_wstFPF;

void ConfigGraphicVariable::setFPF(const std::wstring& _wstFPF)
{
    m_wstFPF = _wstFPF;
}

std::wstring ConfigGraphicVariable::getFPF()
{
    return m_wstFPF;
}

/*
** \}
*/

/*
** Key Tab
** \{
*/

std::map<std::wstring, int> ConfigGraphicVariable::m_mapProperties;

void ConfigGraphicVariable::fillProperties()
{
    if (m_mapProperties.empty())
    {
        m_mapProperties[std::wstring(L"alufunction")]   = 1;
        m_mapProperties[std::wstring(L"auto clear")]    = 2;
        m_mapProperties[std::wstring(L"background")]    = 3;
        m_mapProperties[std::wstring(L"clipoff")]       = 4;
        m_mapProperties[std::wstring(L"clipping")]      = 5;
        m_mapProperties[std::wstring(L"clipgrf")]       = 6;
        m_mapProperties[std::wstring(L"color")]         = 7;
        m_mapProperties[std::wstring(L"colormap")]      = 8;
        m_mapProperties[std::wstring(L"dashes")]        = 9;
        m_mapProperties[std::wstring(L"default")]       = 10;
        m_mapProperties[std::wstring(L"figure")]        = 11;
        m_mapProperties[std::wstring(L"font")]          = 12;
        m_mapProperties[std::wstring(L"font size")]     = 13;
        m_mapProperties[std::wstring(L"foreground")]    = 14;
        m_mapProperties[std::wstring(L"fpf")]           = 15;
        m_mapProperties[std::wstring(L"hidden3d")]      = 16;
        m_mapProperties[std::wstring(L"lastpattern")]   = 17;
        m_mapProperties[std::wstring(L"line mode")]     = 18;
        m_mapProperties[std::wstring(L"line style")]    = 19;
        m_mapProperties[std::wstring(L"mark")]          = 20;
        m_mapProperties[std::wstring(L"mark size")]     = 21;
        m_mapProperties[std::wstring(L"old_style")]     = 22;
        m_mapProperties[std::wstring(L"pattern")]       = 23;
        m_mapProperties[std::wstring(L"pixmap")]        = 24;
        m_mapProperties[std::wstring(L"thickness")]     = 25;
        m_mapProperties[std::wstring(L"use color")]     = 26;
        m_mapProperties[std::wstring(L"viewport")]      = 27;
        m_mapProperties[std::wstring(L"wdim")]          = 28;
        m_mapProperties[std::wstring(L"white")]         = 29;
        m_mapProperties[std::wstring(L"window")]        = 30;
        m_mapProperties[std::wstring(L"wpdim")]         = 31;
        m_mapProperties[std::wstring(L"wpos")]          = 32;
        m_mapProperties[std::wstring(L"wresize")]       = 33;
        //        m_mapProperties[std::wstring(L" ")]             = 34; /* added */
    }
}

int ConfigGraphicVariable::getPropertyValue(wchar_t* _wcsKey)
{
    fillProperties();
    std::map<std::wstring, int>::iterator it;
    it = m_mapProperties.find(_wcsKey);
    if (it != m_mapProperties.end())
    {
        return it->second;
    }

    return 0;
}

bool ConfigGraphicVariable::bPropertyFound(wchar_t* _wcsKey)
{
    fillProperties();
    if (m_mapProperties.find(_wcsKey) != m_mapProperties.end())
    {
        return true;
    }

    return false;
}

/*
** \}
*/


/*
** Colormap
** \{
*/

unsigned short* ConfigGraphicVariable::m_siColormap = NULL;

void ConfigGraphicVariable::initColormap()
{
    if (m_siColormap == NULL)
    {
        m_siColormap = new unsigned short[3 * NUMCOLORS_SCI];

        // default color map
        unsigned short defcolors[] =
        {
            0, 0, 0,                    /* Black: DEFAULTBLACK */
            0, 0, 255,                  /* Blue */
            0, 255, 0,                  /* Green */
            0, 255, 255,                /* Cyan */
            255, 0, 0,                  /* Red */
            255, 0, 255,                /* Magenta */
            255, 255, 0,                /* Yellow */
            255, 255, 255,              /* White: DEFAULTWHITE */
            0, 0, 144,                  /* Blue4 */
            0, 0, 176,                  /* Blue3 */
            0, 0, 208,                  /* Blue2 */
            135, 206, 255,              /* LtBlue */
            0, 144, 0,                  /* Green4 */
            0, 176, 0,                  /* Green3 */
            0, 208, 0,                  /* Green2 */
            0, 144, 144,                /* Cyan4 */
            0, 176, 176,                /* Cyan3 */
            0, 208, 208,                /* Cyan2 */
            144, 0, 0,                  /* Red4 */
            176, 0, 0,                  /* Red3 */
            208, 0, 0,                  /* Red2 */
            144, 0, 144,                /* Magenta4 */
            176, 0, 176,                /* Magenta3 */
            208, 0, 208,                /* Magenta2 */
            128, 48, 0,                 /* Brown4 */
            160, 64, 0,                 /* Brown3 */
            192, 96, 0,                 /* Brown2 */
            255, 128, 128,              /* Pink4 */
            255, 160, 160,              /* Pink3 */
            255, 192, 192,              /* Pink2 */
            255, 224, 224,              /* Pink */
            255, 215, 0                 /* Gold */
        };

        memcpy(m_siColormap, defcolors, 3 * NUMCOLORS_SCI * sizeof(short));
    }
}

unsigned short* ConfigGraphicVariable::getDefaultColormap()
{
    initColormap();
    return m_siColormap;
}

/*
** \}
*/

