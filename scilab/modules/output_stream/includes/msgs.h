/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
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
#ifndef __MSGS_H__
#define __MSGS_H__

#include "dynlib_output_stream.h"
#include "machine.h" // C2F

/**
 * @TODO : comment
 *
 * @param n
 * @param ierr
 * @return <ReturnValue>
 */
OUTPUT_STREAM_IMPEXP int C2F(msgs)(int *n, int *ierr);
/*
 * print a message
 * @param n int message number see modules/output_stream/src/c/msgs.c
 * for the list of messages
 * some message append a buffer to the message (@see printToBuffer)
 */
OUTPUT_STREAM_IMPEXP void Msgs(int n, int ierr);


/*
 * print to the char buffer used to add custom info to some messages
 * (@see Msgs). Buffer is of size bsize as #defined in stack-def.h (i.e. 4096)
 *
 * @param same as sprintf
 * @return same as sprintf
 */
int printToBuffer(const char* format, ...);

#endif /* __MSGS_H__ */
