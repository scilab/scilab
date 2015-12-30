/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __SCILABWRITE_HXX__
#define __SCILABWRITE_HXX__

#define SPACES_LIST "      "

#include "dynlib_output_stream.h"

#ifdef __cplusplus
extern "C"
{
#endif

typedef void (*SCILAB_OUTPUT_METHOD)(const char *text);

OUTPUT_STREAM_IMPEXP void setScilabOutputMethod(SCILAB_OUTPUT_METHOD writer);

OUTPUT_STREAM_IMPEXP void scilabWrite(const char* _pstText);

OUTPUT_STREAM_IMPEXP void scilabWriteW(const wchar_t* _pwstText);

OUTPUT_STREAM_IMPEXP void scilabError(const char* _pstText);
OUTPUT_STREAM_IMPEXP void scilabErrorW(const wchar_t* _pwstText);

/* functions write in "console" without take care of mode*/
OUTPUT_STREAM_IMPEXP void scilabForcedWriteW(const wchar_t* _pwsText);
OUTPUT_STREAM_IMPEXP void scilabForcedWrite(const char* _pstText);

#ifdef __cplusplus
}
#endif

#endif /* __SCILABWRITE_HXX__ */
