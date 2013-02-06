/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/

#include "filemanager.hxx"

extern "C"
{
#include <stdio.h>
#include "mtell.h"
#include "filesmanagement.h"
#include "localization.h"
#include "MALLOC.h"
}

/*--------------------------------------------------------------------------*/
long int mtell(int fd)
{
    File* pF        = FileManager::getFile(fd);
    long int offset = -1;

    if (pF == NULL)
    {
        return offset;
    }

#ifdef _MSC_VER
#if _WIN64
    offset = (long)_ftelli64(pF->getFiledesc()) ;
#else
    offset = ftell(pF->getFiledesc()) ;
#endif
#else
    offset = ftell(pF->getFiledesc()) ;
#endif

    return offset;
}
/*--------------------------------------------------------------------------*/
