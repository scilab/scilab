/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
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

#include "filemanager.hxx"

extern "C"
{
#include <stdio.h>
#include "mtell.h"
#include "localization.h"
#include "sci_malloc.h"
}

/*--------------------------------------------------------------------------*/
long long mtell(int fd)
{
    types::File* pF = FileManager::getFile(fd);
    long long offset = -1;

    if (pF == NULL)
    {
        return offset;
    }

    FILE* fa = pF->getFiledesc();
    if (fa == NULL)
    {
        return -1;
    }

#ifdef _MSC_VER
#if _WIN64
    offset = (long long)_ftelli64(fa);
#else
    offset = (long long)ftell(fa);
#endif
#else
    offset = (long long)ftell(fa);
#endif

    return offset;
}
/*--------------------------------------------------------------------------*/
