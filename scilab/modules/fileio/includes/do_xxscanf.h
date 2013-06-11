/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __DO_XXSCANF_H__
#define __DO_XXSCANF_H__

#include <stdio.h>
#include "dynlib_fileio.h"

/*if maxscan is increased don't forget to change the (*xxscanf)(......)
in do_xxscanf procedure */
#define MAXSCAN 100
#define MAX_STR 1024

typedef union
{
    wchar_t * c;
    long unsigned int lui;
    short unsigned int sui;
    unsigned int ui;
    long int li;
    short int si;
    int i;
    double lf;
    float f;
} rec_entry;

typedef union
{
    double d;
    wchar_t * s;
} entry;
typedef enum {SF_C, SF_S, SF_LUI, SF_SUI, SF_UI, SF_LI, SF_SI, SF_I, SF_LF, SF_F} sfdir;

FILEIO_IMPEXP int do_xxscanf (wchar_t *fname, FILE *fp, wchar_t *format, int *nargs, wchar_t *strv, int *retval, rec_entry *buf, sfdir *type);
#endif /* __DO_XXSCANF_H__ */

