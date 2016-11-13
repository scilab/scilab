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

std::string ConfigGraphicVariable::m_stFPF;

void ConfigGraphicVariable::setFPF(const std::string& _stFPF)
{
    m_stFPF = _stFPF;
}

std::string ConfigGraphicVariable::getFPF()
{
    return m_stFPF;
}

/*
** \}
*/

/*
** Key Tab
** \{
*/

std::map<std::string, int> ConfigGraphicVariable::m_mapProperties;

void ConfigGraphicVariable::fillProperties()
{
    if (m_mapProperties.empty())
    {
        m_mapProperties["alufunction"]   = 1;
        m_mapProperties["auto clear"]    = 2;
        m_mapProperties["background"]    = 3;
        m_mapProperties["clipoff"]       = 4;
        m_mapProperties["clipping"]      = 5;
        m_mapProperties["clipgrf"]       = 6;
        m_mapProperties["color"]         = 7;
        m_mapProperties["colormap"]      = 8;
        m_mapProperties["dashes"]        = 9;
        m_mapProperties["default"]       = 10;
        m_mapProperties["figure"]        = 11;
        m_mapProperties["font"]          = 12;
        m_mapProperties["font size"]     = 13;
        m_mapProperties["foreground"]    = 14;
        m_mapProperties["fpf"]           = 15;
        m_mapProperties["hidden3d"]      = 16;
        m_mapProperties["lastpattern"]   = 17;
        m_mapProperties["line mode"]     = 18;
        m_mapProperties["line style"]    = 19;
        m_mapProperties["mark"]          = 20;
        m_mapProperties["mark size"]     = 21;
        m_mapProperties["old_style"]     = 22;
        m_mapProperties["pattern"]       = 23;
        m_mapProperties["pixmap"]        = 24;
        m_mapProperties["thickness"]     = 25;
        m_mapProperties["use color"]     = 26;
        m_mapProperties["viewport"]      = 27;
        m_mapProperties["wdim"]          = 28;
        m_mapProperties["white"]         = 29;
        m_mapProperties["window"]        = 30;
        m_mapProperties["wpdim"]         = 31;
        m_mapProperties["wpos"]          = 32;
        m_mapProperties["wresize"]       = 33;
//        m_mapProperties[std::wstring(L" "]             = 34; /* added */
    }
}

int ConfigGraphicVariable::getPropertyValue(const char* _key)
{
    fillProperties();
    std::map<std::string, int>::iterator it;
    it = m_mapProperties.find(_key);
    if(it != m_mapProperties.end())
    {
        return it->second;
    }

    return 0;
}

bool ConfigGraphicVariable::bPropertyFound(const char* _key)
{
    fillProperties();
    if(m_mapProperties.find(_key) != m_mapProperties.end())
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

