/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2011 - Allan CORNET
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

