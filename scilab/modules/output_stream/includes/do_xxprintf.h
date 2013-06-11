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
#ifndef __DO_XXPRINTF_H__
#define __DO_XXPRINTF_H__

#include <stdio.h>
#include "dynlib_output_stream.h"

#define VPTR void *
#define DO_XXPRINTF_RET_END -2
#define DO_XXPRINTF_RET_BUG -1
#define DO_XXPRINTF_FAIL 0
#define DO_XXPRINTF_OK 1
#define DO_XXPRINTF_MEM_LACK -3
#define DO_XXPRINTF_MISMATCH -4
#define DO_XXPRINTF_NOT_ENOUGH_ARGS -5

OUTPUT_STREAM_IMPEXP int do_xxprintf (char *fname, FILE *fp, char *format, int nargs, int argcnt, int lcount, char **strv);
OUTPUT_STREAM_IMPEXP int do_xxprintfW(wchar_t* _pwstName, FILE* _fp, wchar_t* _pwstFormat, int _iArgs, int _iArgCount, int _iLcount, wchar_t** _pwstV);

#endif /* __DO_XXPRINTF_H__ */
