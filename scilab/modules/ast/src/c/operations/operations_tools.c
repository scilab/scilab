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

int GetSize(int* _piDims, int _iDims)
{
    int iSize = 1;
    int i = 0;
    for (i = 0 ; i < _iDims ; i++)
    {
        iSize *= _piDims[i];
    }
    return iSize;
}