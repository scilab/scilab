/*
 *  Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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

/* severity of a message sent to the console, ordered by increasing level */
typedef enum
{
    SCI_MSG_OUTPUT  = 0,
    SCI_MSG_WARNING = 1,
    SCI_MSG_ERROR   = 2
} scilab_print_severity;

typedef void (*SCILAB_OUTPUT_METHOD)(const char *text);
typedef void (*SCILAB_PRINT_METHOD)(const char *text, int severity);

/* single writer receiving every message with its severity, preferred over the legacy per-stream writers below */
OUTPUT_STREAM_IMPEXP void setScilabPrintMethod(SCILAB_PRINT_METHOD writer);

/* legacy writers: severity is flattened to output/error streams; any of these setters disables the SCILAB_PRINT_METHOD writer */
OUTPUT_STREAM_IMPEXP void setScilabOutputMethod(SCILAB_OUTPUT_METHOD writer);
OUTPUT_STREAM_IMPEXP void setScilabErrorStreamMethod(SCILAB_OUTPUT_METHOD writer);
OUTPUT_STREAM_IMPEXP void setScilabOutputStreamMethod(SCILAB_OUTPUT_METHOD writer);

/* unconditional funnel: every console message goes through here, whatever its origin */
OUTPUT_STREAM_IMPEXP void scilab_print(const char* _pstText, int _iSeverity);
OUTPUT_STREAM_IMPEXP void scilab_printW(const wchar_t* _pwstText, int _iSeverity);

OUTPUT_STREAM_IMPEXP void scilabWrite(const char* _pstText);

OUTPUT_STREAM_IMPEXP void scilabWriteW(const wchar_t* _pwstText);

OUTPUT_STREAM_IMPEXP void scilabError(const char* _pstText);
OUTPUT_STREAM_IMPEXP void scilabErrorW(const wchar_t* _pwstText);

OUTPUT_STREAM_IMPEXP void scilabWarning(const char* _pstText);
OUTPUT_STREAM_IMPEXP void scilabWarningW(const wchar_t* _pwstText);

/* functions write in "console" without take care of mode*/
OUTPUT_STREAM_IMPEXP void scilabForcedWriteW(const wchar_t* _pwsText);
OUTPUT_STREAM_IMPEXP void scilabForcedWrite(const char* _pstText);

/**
 * Start capturing all output that would normally go to the console.
 * @param silent if non-zero, output is captured only (not forwarded to the console)
 */
OUTPUT_STREAM_IMPEXP void scilabStartOutputCapture(int silent);

/**
 * Stop capturing output and return the captured text.
 * Caller must free() the returned string.
 * @return captured output as a UTF-8 string (caller must free), or NULL if not capturing
 */
OUTPUT_STREAM_IMPEXP char* scilabStopOutputCapture(void);

#ifdef __cplusplus
}
#endif

#endif /* __SCILABWRITE_HXX__ */
