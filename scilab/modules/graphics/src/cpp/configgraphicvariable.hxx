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

#ifndef __CONFIGGRAPHICVARIABLE_HXX__
#define __CONFIGGRAPHICVARIABLE_HXX__

#include <string>
#include <map>

#define NUMCOLORS_SCI 32

extern "C"
{
#include "dynlib_graphics.h"
}

class GRAPHICS_IMPEXP ConfigGraphicVariable
{
    //FPF
private :
    static std::wstring m_wstFPF;

public :
    static void setFPF(const std::wstring& _wstFPF);
    static std::wstring getFPF();

    //KeyTab_, graphic properties
private :
    static std::map<std::wstring, int> m_mapProperties;
    static void fillProperties();

public :
    static bool bPropertyFound(wchar_t* _wcsKey);
    static int getPropertyValue(wchar_t* _wcsKey);

    //color map
private :
    static unsigned short* m_siColormap;
    static void initColormap();

public :
    static unsigned short* getDefaultColormap();


};

#endif /* !__CONFIGGRAPHICVARIABLE_HXX__ */
