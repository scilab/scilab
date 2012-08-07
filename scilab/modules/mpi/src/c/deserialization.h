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

/**
 * Deserialize a double and return a double Scilab variable
 *
 * @param _pvCtx The runtime context
 * @param _piBuffer The buffer
 * @param _iBufferSize The size of the buffer
 * @return 0 in case of success
 */
int deserialize_double(void *_pvCtx, int *_piBuffer, int _iBufferSize);

/**
 * Deserialize a string and return a string Scilab variable
 *
 * @param _pvCtx The runtime context
 * @param _piBuffer The buffer
 * @param _iBufferSize The size of the buffer
 * @return 0 in case of success
 */
int deserialize_string(void *_pvCtx, int *_piBuffer, int _iBufferSize);

/**
 * Deserialize a boolean and return a boolean Scilab variable
 *
 * @param _pvCtx The runtime context
 * @param _piBuffer The buffer
 * @param _iBufferSize The size of the buffer
 * @return 0 in case of success
 */
int deserialize_boolean(void *_pvCtx, int *_piBuffer, int _iBufferSize);

/**
 * Deserialize an int and return a int Scilab variable
 *
 * @param _pvCtx The runtime context
 * @param _piBuffer The buffer
 * @param _iBufferSize The size of the buffer
 * @return 0 in case of success
 */
int deserialize_int(void *_pvCtx, int *_piBuffer, int _iBufferSize);

/**
 * Deserialize a sparse and return a sparse Scilab variable
 *
 * @param _pvCtx The runtime context
 * @param _piBuffer The buffer
 * @param _iBufferSize The size of the buffer
 * @return 0 in case of success
 */
int deserialize_sparse(void *_pvCtx, int *_piBuffer, int _iBufferSize, BOOL _bData);
