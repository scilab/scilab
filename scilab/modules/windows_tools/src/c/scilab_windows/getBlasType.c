/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2011 - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include <Windows.h>
#include "getBlasType.h"
/*--------------------------------------------------------------------------*/
#define BLASLIBNAME "blasplus.dll"
#define DGEMMCNAME  "dgemm"
#define DGEMMFNAME  "dgemm_"
#define MKLGETVERSIONNAME "MKL_Get_Version"
/*--------------------------------------------------------------------------*/
static blas_type blasType = BLAS_UNKNOW;
static BOOL bBlasDetected = FALSE;
/*--------------------------------------------------------------------------*/
typedef int (*DGEMPROC_C) ();
typedef int (*DGEMPROC_F) ();
typedef int (*MKL_GET_VERSIONPROC) ();
/*--------------------------------------------------------------------------*/
blas_type getBlasType(void)
{
    if (!bBlasDetected)
    {
        HINSTANCE BlasDll = NULL;
        bBlasDetected = TRUE;
        BlasDll = LoadLibrary (BLASLIBNAME);
        if (BlasDll != NULL)
        {
            DGEMPROC_C dynDGEMMPROC_C = (DGEMPROC_C)GetProcAddress(BlasDll, DGEMMCNAME);
            DGEMPROC_F dynDGEMMPROC_F = (DGEMPROC_C)GetProcAddress(BlasDll, "dgemm_");
            MKL_GET_VERSIONPROC dynMKL_GET_VERSIONPROC = (MKL_GET_VERSIONPROC)GetProcAddress(BlasDll, MKLGETVERSIONNAME);
            if (dynMKL_GET_VERSIONPROC)
            {
                blasType = BLAS_MKL;
            }
            else
            {
                if (dynDGEMMPROC_C && dynDGEMMPROC_F)
                {
                    blasType = BLAS_REF;
                }
                else
                {
                    if ((dynDGEMMPROC_F == NULL) && (dynDGEMMPROC_C))
                    {
                        blasType = BLAS_ATLAS;
                    }
                }
            }
            FreeLibrary(BlasDll);
            BlasDll = NULL;
        }
    }
    return blasType;
}
/*--------------------------------------------------------------------------*/

