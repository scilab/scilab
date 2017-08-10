/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#ifndef __INTERNAL_COMMON_API__
#define __INTERNAL_COMMON_API__

/**
*
* @param[in]
* @param
* @return
*/
int getNewVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress);

/**
*
* @param
*/
void createNamedVariable(int *_piVarID);

/**
*
* @param[in]
* @param[in]
* @param[in]
* @return
*/
int updateLstk(int _iNewPos, int _iSCIDataAddress, int _iVarSize);

/**
* @param[in] _piAddress variable address
* @param[in] _iType type to check
* @return 1 for true and 0 for false
*/
int checkVarType(void* _pvCtx, int* _piAddress, int _iType);

/**
* @param[in] _psName variable name
* @param[in] _iType type to check
* @return 1 for true and 0 for false
*/
int checkNamedVarType(void* _pvCtx, const char *_pstName, int _iType);

/**
* @param[in] _psName variable name
* @return 1 for true (variable name is valid) and 0 for false
*/
int checkNamedVarFormat(void* _pvCtx, const char *_pstName);

#endif /* __INTERNAL_COMMON_API__ */

