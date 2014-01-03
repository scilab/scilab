/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2011 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/**
 * Serialize a Scilab variable to mpi data
 *
 * @param _pvCtx The runtime context
 * @param _piBuffer The buffer
 * @param _iBufferSize The size of the buffer
 * @return 0 in case of success
 */

int serialize_to_mpi(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize);
