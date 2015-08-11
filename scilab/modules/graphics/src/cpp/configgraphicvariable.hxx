/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
    static void setFPF(std::wstring _wstFPF);
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
