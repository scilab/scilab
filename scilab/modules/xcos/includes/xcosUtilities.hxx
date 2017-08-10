/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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

#ifndef __XCOS_UTILITIES_HXX__
#define __XCOS_UTILITIES_HXX__

/**
 * Read a single boolean on the stack.
 *
 * @param _pvCtx private api context (opaque structure)
 * @param rhsPosition the position on the stack.
 * @param[out] out the read value.
 * @param fname the function name used for the call.
 * @return status of the operation (<> 0 on error)
 */
int readSingleBoolean(void* pvApiCtx, int rhsPosition, bool* out, const char* fname);

/**
 * Read a single string on the stack.
 *
 * @param _pvCtx private api context (opaque structure)
 * @param rhsPosition the position on the stack.
 * @param[out] out the read value.
 * @param fname the function name used for the call.
 * @return status of the operation (<> 0 on error)
 */
int readSingleString(void* pvApiCtx, int rhsPosition, char** out, const char* fname);

/**
 * Read a vector of string on the stack.
 *
 * @param _pvCtx private api context (opaque structure)
 * @param rhsPosition the position on the stack.
 * @param[out] out the read value.
 * @param[out] vectorLength the length of the vector.
 * @param fname the function name used for the call.
 * @return status of the operation (<> 0 on error)
 */
int readVectorString(void* pvApiCtx, int rhsPosition, char*** out, int* vectorLength, char* fname);

/**
 * Free the allocated vector of strings after the readVectorString call
 *
 * @param str the vector of strings
 * @param len the length of the vector
 */
void releaseVectorString(char** str, int len);

#endif /* !__XCOS_UTILITIES_HXX__ */
