/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011-2011 - DIGITEO - Antoine ELIAS
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

/**
 * Serialize a Scilab variable to mpi data
 *
 * @param _pvCtx The runtime context
 * @param _piBuffer The buffer
 * @param _iBufferSize The size of the buffer
 * @return 0 in case of success
 */

int serialize_to_mpi(void *_pvCtx, int *_piAddr, int **_piBuffer, int *_piBufferSize);
