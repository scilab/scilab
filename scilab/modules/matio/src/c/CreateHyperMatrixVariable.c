/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * Copyright (C) 2010 - DIGITEO - Yann COLLETTE
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "CreateMatlabVariable.h"
#include "stack-c.h"
#include "api_scilab.h"

int CreateHyperMatrixVariable(int iVar, const char *type, int *iscomplex, int * rank, int *dims, double *realdata, double *complexdata, int * parent, int item_position)
{
    static const char *tlistFields[] = {"hm", "dims","entries"};
    int nbRow = 0, nbCol = 0;
    int K = 0, i;
    int * hm_addr = NULL;
    SciErr sciErr;
    
    /* Used for integer hypermatrices */
    char * tmp_int8 = NULL;
    short * tmp_int16 = NULL;
    int * tmp_int32 = NULL;
    unsigned char * tmp_uint8 = NULL;
    unsigned short * tmp_uint16 = NULL;
    unsigned int * tmp_uint32 = NULL;
#ifdef __SCILAB_INT64__
    long long * tmp_int64 = NULL;
    unsigned long long * tmp_uint64 = NULL;
#endif
    
    if (parent==NULL)
    {
        sciErr = createMList(pvApiCtx, iVar, 3, &hm_addr);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return FALSE;
        }
    }
    else
    {
        sciErr = createMListInList(pvApiCtx, iVar, parent, item_position, 3, &hm_addr);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return FALSE;
        }
    }
    
    /* mlist fields */
    nbRow = 1;
    nbCol = 3;
    sciErr = createMatrixOfStringInList(pvApiCtx, iVar, hm_addr, 1, nbRow, nbCol, (const char **)tlistFields);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return FALSE;
    }
    
    /* hm dimensions */
    sciErr = createMatrixOfInteger32InList(pvApiCtx, iVar, hm_addr, 2, 1, *rank, dims);
    if(sciErr.iErr)
    {
        printError(&sciErr, 0);
        return FALSE;
    }
    
    /* hm entries */
    nbRow = 1;
    nbCol = 1;
    for (K=0; K<*rank; K++)
    {
        nbRow *= dims[K];
    }
    
    if (strcmp(type,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE) == 0)
    {
        switch(*iscomplex)
        {
            case I_CHAR:
                tmp_int8 = (char *)MALLOC(nbRow*nbCol*sizeof(char));
                if (tmp_int8 == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "CreateHyperMatrixVariable");
                    return FALSE;
                }
                for(i=0; i<nbRow*nbCol; i++)
                {
                    tmp_int8[i] = ((char *)realdata)[i];
                }
                sciErr = createMatrixOfInteger8InList(pvApiCtx, iVar, hm_addr, 3, nbRow, nbCol, tmp_int8);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(tmp_int8);
                    return FALSE;
                }
                FREE(tmp_int8);
                break;
            case I_INT16:
                tmp_int16 = (short *)MALLOC(nbRow*nbCol*sizeof(short));
                if (tmp_int16 == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "CreateHyperMatrixVariable");
                    return FALSE;
                }
                for(i=0; i<nbRow*nbCol; i++)
                {
                    tmp_int16[i] = ((short *)realdata)[i];
                }
                sciErr = createMatrixOfInteger16InList(pvApiCtx, iVar, hm_addr, 3, nbRow, nbCol, tmp_int16);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(tmp_int16);
                    return FALSE;
                }
                
                FREE(tmp_int16);
                break;
            case I_INT32:
                tmp_int32 = (int *)MALLOC(nbRow*nbCol*sizeof(int));
                if (tmp_int32 == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "CreateHyperMatrixVariable");
                    return FALSE;
                }
                for(i=0; i<nbRow*nbCol; i++)
                {
                    tmp_int32[i] = ((int *)realdata)[i];
                }
                sciErr = createMatrixOfInteger32InList(pvApiCtx, iVar, hm_addr, 3, nbRow, nbCol, tmp_int32);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(tmp_int32);
                    return FALSE;
                }
                FREE(tmp_int32);
                break;
#ifdef __SCILAB_INT64__
            case I_INT64:
                tmp_int64 = (long long *)MALLOC(nbRow*nbCol*sizeof(long long));
                if (tmp_int64 == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "CreateHyperMatrixVariable");
                    return FALSE;
                }
                for(i=0; i<nbRow*nbCol; i++)
                {    
                    tmp_int64[i] = ((long long *)realdata)[i];
                }
                qciErr = createMatrixOfInteger64InList(pvApiCtx, iVar, hm_addr, 3, nbRow, nbCol, tmp_int64);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(tmp_int64);
                    return FALSE;
                }
                FREE(tmp_int64);
                break;
#endif
            case I_UCHAR:
                tmp_uint8 = (unsigned char *)MALLOC(nbRow*nbCol*sizeof(unsigned char));
                if (tmp_uint8 == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "CreateHyperMatrixVariable");
                    return FALSE;
                }
                for(i=0; i<nbRow*nbCol; i++)
                {
                    tmp_uint8[i] = ((unsigned char *)realdata)[i];
                }
                sciErr = createMatrixOfUnsignedInteger8InList(pvApiCtx, iVar, hm_addr, 3, nbRow, nbCol, tmp_uint8);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(tmp_uint8);
                    return FALSE;
                }
                FREE(tmp_uint8);
                break;
            case I_UINT16:
                tmp_uint16 = (unsigned short *)MALLOC(nbRow*nbCol*sizeof(unsigned short));
                if (tmp_uint16 == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "CreateHyperMatrixVariable");
                    return FALSE;
                }
                for(i=0; i<nbRow*nbCol; i++)
                {
                    tmp_uint16[i] = ((unsigned short *)realdata)[i];
                }
                sciErr = createMatrixOfUnsignedInteger16InList(pvApiCtx, iVar, hm_addr, 3, nbRow, nbCol, tmp_uint16);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(tmp_uint16);
                    return FALSE;
                }
                FREE(tmp_uint16);
                break;
            case I_UINT32:
                tmp_uint32 = (unsigned int *)MALLOC(nbRow*nbCol*sizeof(unsigned int));
                if (tmp_uint32 == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "CreateHyperMatrixVariable");
                    return FALSE;
                }
                for(i=0;i<nbRow*nbCol; i++)
                {
                    tmp_uint32[i] = ((unsigned int *)realdata)[i];
                }
                sciErr = createMatrixOfUnsignedInteger32InList(pvApiCtx, iVar, hm_addr, 3, nbRow, nbCol, tmp_uint32);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(tmp_uint32);
                    return FALSE;
                }
                FREE(tmp_uint32);
                break;
#ifdef __SCILAB_INT64__
            case I_UINT64:
                tmp_uint64 = (unsigned long long *)MALLOC(nbRow*nbCol*sizeof(unsigned long long));
                if (tmp_uint64 == NULL)
                {
                    Scierror(999, _("%s: No more memory.\n"), "CreateHyperMatrixVariable");
                    return FALSE;
                }
                for(i=0; i<nbRow*nbCol; i++)
                {
                    tmp_uint64[i] = ((unsigned long long *)realdata)[i];
                }
                sciErr = createMatrixOfUnsignedInteger64InList(pvApiCtx, iVar, hm_addr, 3, nbRow, nbCol, tmp_uint64);
                if(sciErr.iErr)
                {
                    printError(&sciErr, 0);
                    FREE(tmp_uint64);
                    return FALSE;
                }
                FREE(tmp_uint64);
                break;
#endif
        }
    }
    else if (strcmp(type, MATRIX_OF_BOOLEAN_DATATYPE) == 0)
    {
        sciErr = createMatrixOfDoubleInList(pvApiCtx, iVar, hm_addr, 3, nbRow, nbCol, realdata);
        if(sciErr.iErr)
        {
            printError(&sciErr, 0);
            return FALSE;
        }
    }
    else
    {
        if (*iscomplex == 0)
        {
            sciErr = createMatrixOfDoubleInList(pvApiCtx, iVar, hm_addr, 3, nbRow, nbCol, realdata);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return FALSE;
            }
        }
        else
        {
            sciErr = createComplexMatrixOfDoubleInList(pvApiCtx, iVar, hm_addr, 3, nbRow, nbCol, realdata, complexdata);
            if(sciErr.iErr)
            {
                printError(&sciErr, 0);
                return FALSE;
            }
        }
    }
    
    return TRUE;
}
