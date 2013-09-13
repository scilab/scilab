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
#include <errno.h>
#include "mseek.h"
#include "sciprint.h"
#include "filesmanagement.h"
#include "localization.h"
#include "charEncoding.h"
#include "MALLOC.h"
}
/*--------------------------------------------------------------------------*/

int mseek(int fd, long long offset, int iflag)
{
#if (defined(sun) && !defined(SYSV)) || defined(sgi)
    int irep;
#endif

    File* pF = FileManager::getFile(fd);
    if (pF == NULL)
    {
        sciprint(_("%s: No input file associated to logical unit %d.\n"), "mseek", fd);
        return 1;
    }
    FILE* fa = pF->getFiledesc();

    if (fa == NULL)
    {
        return 1;
    }

    if (iflag != SEEK_SET && iflag != SEEK_CUR && iflag != SEEK_END)
    {
        return 1;
    }

#if (defined(sun) && !defined(SYSV)) || defined(sgi)
    irep = fseek(fa, (long)offset, iflag) ;
    if (irep != 0 )
    {
        return 1;
    }
    else
    {
        return 0;
    }
#else
#ifdef _MSC_VER
#if _WIN64
    if (_fseeki64(fa, offset, iflag) == -1 )
#else
    if (fseek(fa, (long)offset, iflag) == -1 )
#endif
#else
    if (fseek(fa, (long)offset, iflag) == -1)
#endif
    {
        return errno; // global variable produced by fseek
    }
    else
    {
        return 0;
    }
#endif
}
/*--------------------------------------------------------------------------*/
