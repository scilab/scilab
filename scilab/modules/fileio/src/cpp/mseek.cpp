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
#include <errno.h>
#include "mseek.h"
#include "sciprint.h"
#include "localization.h"
#include "charEncoding.h"
#include "sci_malloc.h"
}
/*--------------------------------------------------------------------------*/

int mseek(int fd, long long offset, int iflag)
{
#if (defined(sun) && !defined(SYSV)) || defined(sgi)
    int irep;
#endif

    types::File* pF = FileManager::getFile(fd);
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
