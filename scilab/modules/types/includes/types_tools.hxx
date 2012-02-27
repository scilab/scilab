/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2011 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __TYPES_TOOLS_HXX__
#define __TYPES_TOOLS_HXX__

#include "dynlib_types.h"
#include "types.hxx"

namespace types
{
    TYPES_IMPEXP int checkIndexesArguments(InternalType* _pRef, typed_list* _pArgsIn, typed_list* _pArgsOut, int* _piMaxDim, int* _piCountDim);
    TYPES_IMPEXP void getIndexesWithDims(int _iIndex, int* _piIndexes, int* _piDims, int _iDims);
    TYPES_IMPEXP int getIndexWithDims(int* _piIndexes, int* _piDims, int _iDims);
}

#endif /* !__TYPES_TOOLS_HXX__ */
