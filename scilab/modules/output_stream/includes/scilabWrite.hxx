/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
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

#ifndef __SCILABWRITE_HXX__
#define __SCILABWRITE_HXX__

#define SPACES_LIST L"      "

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
