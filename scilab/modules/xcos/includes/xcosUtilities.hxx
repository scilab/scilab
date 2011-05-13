/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
int readSingleBoolean(void* _pvCtx, int rhsPosition, bool* out, const char* fname);

/**
 * Read a single string on the stack.
 *
 * @param _pvCtx private api context (opaque structure)
 * @param rhsPosition the position on the stack.
 * @param[out] out the read value.
 * @param fname the function name used for the call.
 * @return status of the operation (<> 0 on error)
 */
int readSingleString(void* _pvCtx, int rhsPosition, char** out, const char* fname);

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
int readVectorString(void* _pvCtx, int rhsPosition, char*** out, int* vectorLength, char* fname);

#endif /* !__XCOS_UTILITIES_HXX__ */
