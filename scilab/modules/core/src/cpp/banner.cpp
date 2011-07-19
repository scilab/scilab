/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "banner.hxx"
#include "yaspio.hxx"

extern "C"
{
#include <string.h>
#include <math.h>
#include "version.h"
#include "localization.h"
#include "charEncoding.h"
}

void banner(void)
{
    int i;
    wchar_t *line = L"        ___________________________________________        ";
    int startVersion = (int)(floor((double)(wcslen(line)/2)) - floor((double)(wcslen(SCI_VERSION_WIDE_STRING)/2)));

    YaspForcedWriteW(line);
    YaspForcedWriteW(L"\n");

    /* To center the version name */
    for(i = 0 ; i < startVersion ; i++ )
    {
        YaspForcedWriteW(L" ");
    }

    YaspForcedWriteW(SCI_VERSION_WIDE_STRING);
    YaspForcedWriteW(L"\n\n");

    YaspForcedWriteW(_W("                 Consortium Scilab (DIGITEO)\n"));

    YaspForcedWriteW(_W("               Copyright (c) 1989-2011 (INRIA)\n"));
    YaspForcedWriteW(_W("               Copyright (c) 1989-2007 (ENPC)\n"));
    YaspForcedWriteW(line);
    YaspForcedWriteW(L"\n");
    YaspForcedWriteW(L"\n");
    YaspForcedWriteW(L"                     -*- THIS IS YaSp -*-\n");
    YaspForcedWriteW(L"\n");
    YaspForcedWriteW(line);
    YaspForcedWriteW(L"\n");

#if ( defined(_MSC_VER) && ( (_MSC_VER >= 1200) && (_MSC_VER < 1300) ) )
    YaspForcedWriteW(L"\n\n");
    YaspForcedWriteW(_W("Warning: the operational team of the Scilab Consortium\ndoesn't provide and doesn't support this version of Scilab built with\n"));
    YaspForcedWriteW(L"   ");
#endif
}
