/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2011 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

int serialize_double(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize);
int serialize_string(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize);
int serialize_boolean(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize);
int serialize_int(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize);
int serialize_sparse(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize, BOOL _bData);
