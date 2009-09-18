/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Scilab Consortium Operational Team
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_variable.h"

            
int iTab = 0;

void insert_indent(void)
{
    int i = 0;
    for(i = 0 ; i < iTab ; i++)
    {
        sciprint("\t");
    }
}


int get_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_list_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_double_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_poly_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_boolean_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_sparse_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_bsparse_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_integer_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_string_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);
int get_pointer_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos);

int commom_read(char *fname,unsigned long fname_len)
{
    int iRet        = 0;
    int iItem       = 0;

    int *piAddr     = NULL;

    CheckRhs(1,1);

    iRet = getVarAddressFromPosition(1, &piAddr);
    if(iRet)
    {
        return 1;
    }

    iRet = get_info(1, NULL, piAddr, 0);
    if(iRet)
    {
        return 1;
    }

    LhsVar(1) = 0;
    return 0;
}

int get_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    int iRet        = 0;
    int iType       = 0;

    if(_piAddr == NULL)
    {
        return 1;
    }

    iType = getVarType(_piAddr);
    switch(iType)
    {
    case sci_matrix :
        iRet = get_double_info(_iRhs, _piParent, _piAddr, _iItemPos);
        break;
    case sci_poly :
        iRet = get_poly_info(_iRhs, _piParent, _piAddr, _iItemPos);
        break;
    case sci_boolean :
        iRet = get_boolean_info(_iRhs, _piParent, _piAddr, _iItemPos);
        break;
    case sci_sparse :
        iRet = get_sparse_info(_iRhs, _piParent, _piAddr, _iItemPos);
        break;
    case sci_boolean_sparse :
        iRet = get_bsparse_info(_iRhs, _piParent, _piAddr, _iItemPos);
        break;
    case sci_ints :
        iRet = get_integer_info(_iRhs, _piParent, _piAddr, _iItemPos);
        break;
    case sci_strings :
        iRet = get_string_info(_iRhs, _piParent, _piAddr, _iItemPos);
        break;
    case sci_list :
        insert_indent();
        sciprint("List ");
        iRet = get_list_info(_iRhs, _piParent, _piAddr, _iItemPos);
        break;
    case sci_tlist :
        insert_indent();
        sciprint("TList ");
        iRet = get_list_info(_iRhs, _piParent, _piAddr, _iItemPos);
        break;
    case sci_mlist :
        insert_indent();
        sciprint("MList ");
        iRet = get_list_info(_iRhs, _piParent, _piAddr, _iItemPos);
        break;
    case sci_pointer :
        iRet = get_pointer_info(_iRhs, _piParent, _piAddr, _iItemPos);
        break;
    default :
        return 1;
    }
    return iRet;
}

int get_list_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    int i;
    int iRet        = 0;
    int iItem       = 0;
    int* piChild    = NULL;

    iRet = getListItemNumber(_piAddr, &iItem);
    if(iRet)
    {
        return 1;
    }

    sciprint("(%d)\n", iItem);
    for(i = 0 ; i < iItem ; i++)
    {
        iRet = getListItemAddress(_piAddr, i + 1, &piChild);
        if(iRet)
        {
            return 1;
        }

        iTab++;
        iRet = get_info(_iRhs, _piAddr, piChild, i + 1);
        iTab--;
    }

    return iRet;
}

int get_double_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    int iRet            = 0;
    int iRows           = 0;
    int iCols           = 0;

    double* pdblReal    = NULL;
    double* pdblImg     = NULL;

    if(_iItemPos == 0)
    {//not in list
        if(isVarComplex(_piAddr))
        {
            iRet = getComplexMatrixOfDouble(_piAddr, &iRows, &iCols, &pdblReal, &pdblImg);
        }
        else
        {
            iRet = getMatrixOfDouble(_piAddr, &iRows, &iCols, &pdblReal);
        }
    }
    else
    {
        if(isVarComplex(_piAddr))
        {
            iRet = getComplexMatrixOfDoubleInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &pdblReal, &pdblImg);
        }
        else
        {
            iRet = getMatrixOfDoubleInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &pdblReal);
        }
    }

    if(iRet)
    {
        return 1;
    }

    insert_indent();
    sciprint("Double (%d x %d)\n", iRows, iCols);

    return iRet;
}

int get_poly_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    int i;
    int iRet            = 0;
    int iLen            = 0;
    int iRows           = 0;
    int iCols           = 0;

    char pstVar[16];
    int* piCoeff        = NULL;
    double** pdblReal   = NULL;
    double** pdblImg    = NULL;

    getPolyVariableName(_piAddr, pstVar, &iLen);

    if(_iItemPos == 0)
    {//not in list
        iRet = getMatrixOfPoly(_piAddr, &iRows, &iCols, NULL, NULL);
        if(iRet)
        {
            return 1;
        }

        piCoeff     = (int*)malloc(sizeof(int) * iRows * iCols);
        iRet = getMatrixOfPoly(_piAddr, &iRows, &iCols, piCoeff, NULL);
        if(iRet)
        {
            return 1;
        }

        pdblReal    = (double**)malloc(sizeof(double*) * iRows * iCols);
        pdblImg     = (double**)malloc(sizeof(double*) * iRows * iCols);
        for(i = 0 ; i < iRows * iCols ; i++)
        {
            pdblReal[i] = (double*)malloc(sizeof(double) * piCoeff[i]);
            pdblImg[i]  = (double*)malloc(sizeof(double) * piCoeff[i]);
        }

        if(isVarComplex(_piAddr))
        {
            iRet = getComplexMatrixOfPoly(_piAddr, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
            if(iRet)
            {
            return 1;
            }
        }
        else
        {
            iRet = getMatrixOfPoly(_piAddr, &iRows, &iCols, piCoeff, pdblReal);
            if(iRet)
            {
                return 1;
            }
        }
    }
    else
    {
        iRet = getMatrixOfPolyInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, NULL, NULL);
        if(iRet)
        {
            return 1;
        }

        piCoeff = (int*)malloc(sizeof(int) * iRows * iCols);
        iRet = getMatrixOfPolyInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, piCoeff, NULL);
        if(iRet)
        {
            return 1;
        }

        pdblReal    = (double**)malloc(sizeof(double*) * iRows * iCols);
        pdblImg     = (double**)malloc(sizeof(double*) * iRows * iCols);
        for(i = 0 ; i < iRows * iCols ; i++)
        {
            pdblReal[i] = (double*)malloc(sizeof(double) * piCoeff[i]);
            pdblImg[i]  = (double*)malloc(sizeof(double) * piCoeff[i]);
        }

        if(isVarComplex(_piAddr))
        {
            iRet = getComplexMatrixOfPolyInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, piCoeff, pdblReal, pdblImg);
        }
        else
        {
            iRet = getMatrixOfPolyInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, piCoeff, pdblReal);
        }
    }

    if(iRet)
    {
        return 1;
    }

    insert_indent();
    sciprint("Poly  (%d x %d), varname : \'%s\'\n", iRows, iCols, pstVar);

    for(i = 0 ; i < iRows * iCols ; i++)
    {
        free(pdblReal[i]);
        free(pdblImg[i]);
    }
    free(pdblReal);
    free(pdblImg);
    free(piCoeff);
    return 0;
}

int get_boolean_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    int iRet        = 0;
    int iRows       = 0;
    int iCols       = 0;

    int* piBool     = NULL;

    if(_iItemPos == 0)
    {
        iRet = getMatrixOfBoolean(_piAddr, &iRows, &iCols, &piBool);
    }
    else
    {
        iRet = getMatrixOfBooleanInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &piBool);
    }

    if(iRet)
    {
        return 1;
    }

    insert_indent();
    sciprint("Boolean (%d x %d)\n", iRows, iCols);
    return 0;
}

int get_sparse_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    int iRet            = 0;
    int iRows           = 0;
    int iCols           = 0;
    int iItem           = 0;

    int* piNbRow        = NULL;
    int* piColPos       = NULL;

    double* pdblReal    = NULL;
    double* pdblImg     = NULL;

    if(_iItemPos == 0)
    {//Not in list
        if(isVarComplex(_piAddr))
        {
            iRet = getComplexSparseMatrix(_piAddr, &iRows, &iCols, &iItem, &piNbRow, &piColPos, &pdblReal, &pdblImg);
        }
        else
        {
            iRet = getSparseMatrix(_piAddr, &iRows, &iCols, &iItem, &piNbRow, &piColPos, &pdblReal);
        }
    }
    else
    {
        if(isVarComplex(_piAddr))
        {
            iRet = getComplexSparseMatrixInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &iItem, &piNbRow, &piColPos, &pdblReal, &pdblImg);
        }
        else
        {
            iRet = getSparseMatrixInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &iItem, &piNbRow, &piColPos, &pdblReal);
        }
    }

    insert_indent();
    sciprint("Sparse (%d x %d), Item(s) : %d \n", iRows, iCols, iItem);
    return 0;
}

int get_bsparse_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    int iRet        = 0;
    int iRows       = 0;
    int iCols       = 0;
    int iItem       = 0;

    int* piNbRow    = NULL;
    int* piColPos   = NULL;

    if(_iItemPos == 0)
    {//Not in list
        iRet = getBooleanSparseMatrix(_piAddr, &iRows, &iCols, &iItem, &piNbRow, &piColPos);
    }
    else
    {
        iRet = getBooleanSparseMatrixInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &iItem, &piNbRow, &piColPos);
    }

    if(iRet)
    {
        return 1;
    }
    
    insert_indent();
    sciprint("Boolean Sparse (%d x %d), Item(s) : %d \n", iRows, iCols, iItem);
    return 0;
}

int get_integer_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    int iRet                = 0;
    int iPrec               = 0;
    int iRows               = 0;
    int iCols               = 0;

    char* pcData            = NULL;
    short* psData           = NULL;
    int* piData             = NULL;
    unsigned char* pucData  = NULL;
    unsigned short* pusData = NULL;
    unsigned int* puiData   = NULL;

    if(_iItemPos == 0)
    {//Not in list
        iRet = getMatrixOfIntegerPrecision(_piAddr, &iPrec);
        if(iRet)
        {
            return 1;
        }

        switch(iPrec)
        {
        case SCI_INT8 : 
            iRet = getMatrixOfInteger8(_piAddr, &iRows, &iCols, &pcData);
            break;
        case SCI_INT16 : 
            iRet = getMatrixOfInteger16(_piAddr, &iRows, &iCols, &psData);
            break;
        case SCI_INT32 : 
            iRet = getMatrixOfInteger32(_piAddr, &iRows, &iCols, &piData);
            break;
        case SCI_UINT8 : 
            iRet = getMatrixOfUnsignedInteger8(_piAddr, &iRows, &iCols, &pucData);
            break;
        case SCI_UINT16 : 
            iRet = getMatrixOfUnsignedInteger16(_piAddr, &iRows, &iCols, &pusData);
            break;
        case SCI_UINT32 : 
            iRet = getMatrixOfUnsignedInteger32(_piAddr, &iRows, &iCols, &puiData);
            break;
        default :
            return 1;
        }
    }
    else
    {
        iRet = getMatrixOfIntegerPrecision(_piAddr, &iPrec);
        if(iRet)
        {
            return 1;
        }

        switch(iPrec)
        {
        case SCI_INT8 : 
            iRet = getMatrixOfInteger8InList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &pcData);
            break;
        case SCI_INT16 : 
            iRet = getMatrixOfInteger16InList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &psData);
            break;
        case SCI_INT32 : 
            iRet = getMatrixOfInteger32InList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &piData);
            break;
        case SCI_UINT8 : 
            iRet = getMatrixOfUnsignedInteger8InList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &pucData);
            break;
        case SCI_UINT16 : 
            iRet = getMatrixOfUnsignedInteger16InList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &pusData);
            break;
        case SCI_UINT32 : 
            iRet = getMatrixOfUnsignedInteger32InList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, &puiData);
            break;
        default :
            return 1;
        }
    }

    if(iRet)
    {
        return 1;
    }

    insert_indent();
    if(iPrec > 10)
    {
        sciprint("Unsigned ");
    }

    sciprint("Integer %d bits (%d x %d)\n", (iPrec % 10) * 8, iRows, iCols);

    return 0;
}

int get_string_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    int i;
    int iRet        = 0;
    int iRows       = 0;
    int iCols       = 0;
    int* piLen      = NULL;

    char **pstData  = NULL;

    if(_iItemPos == 0)
    {//Not in list
        iRet = getMatrixOfString(_piAddr, &iRows, &iCols, NULL, NULL);
        if(iRet)
        {
            return 1;
        }

        piLen = (int*)malloc(sizeof(int) * iRows * iCols);
        iRet = getMatrixOfString(_piAddr, &iRows, &iCols, piLen, NULL);
        if(iRet)
        {
            return 1;
        }

        pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
        for(i = 0 ; i < iRows * iCols ; i++)
        {
            pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }

        iRet = getMatrixOfString(_piAddr, &iRows, &iCols, piLen, pstData);
        if(iRet)
        {
            return 1;
        }
    }
    else
    {
        iRet = getMatrixOfStringInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, NULL, NULL);
        if(iRet)
        {
            return 1;
        }

        piLen = (int*)malloc(sizeof(int) * iRows * iCols);
        iRet = getMatrixOfStringInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, piLen, NULL);
        if(iRet)
        {
            return 1;
        }

        pstData = (char**)malloc(sizeof(char*) * iRows * iCols);
        for(i = 0 ; i < iRows * iCols ; i++)
        {
            pstData[i] = (char*)malloc(sizeof(char) * (piLen[i] + 1));//+ 1 for null termination
        }

        iRet = getMatrixOfStringInList(_iRhs, _piParent, _iItemPos, &iRows, &iCols, piLen, pstData);
        if(iRet)
        {
            return 1;
        }
    }

    if(iRet)
    {
        return 1;
    }

    insert_indent();
    sciprint("Strings (%d x %d)\n", iRows, iCols);

    return 0;
}

int get_pointer_info(int _iRhs, int* _piParent, int *_piAddr, int _iItemPos)
{
    int iRet        = 0;
    void* pvPtr     = NULL;
    double* myPtr   = NULL;

    if(_iItemPos == 0)
    {
        iRet = getPointer(_piAddr, &pvPtr);
    }
    else
    {
        iRet = getPointerInList(_iRhs, _piParent, _iItemPos, &pvPtr);
    }

    if(iRet)
    {
    return 1;
    }

    myPtr = (double *) ((unsigned long int)((double*)pvPtr)[0]);
    if(myPtr == NULL)
    {
        return 1;
    }

    insert_indent();
    sciprint("Pointer : 0x%08X\n", myPtr);

    return 0;
}
        
        
