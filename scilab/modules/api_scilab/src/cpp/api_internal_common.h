/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
*
* @param[in]
* @param[in]
* @param[in]
* @param[in]
* @return
*/
int updateInterSCI(int _iVar, char _cType, int _iSCIAddress, int _iSCIDataAddress);

#endif /* __INTERNAL_COMMON_API__ */

