/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2013 - Scilab Enterprises - Cedric DELAMARRE
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
#include "optimization_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "tlist.hxx"
#include "numericconstants.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
#include "elem_common.h"
#include "sci_malloc.h"

    void C2F(rdmpsz)(int*, int*, int*, int*, int*, char*, int*);
    int  C2F(clunit)(int*, char*, int*, int);
    void C2F(rdmps1)(int*, char*, int*, int*, int*, int*, int*, int*, int*,
                     double*, double*, double*, char*, char*, char*, char*,
                     char*, int*, char*, char*, int*, int*, int*, int*, int*,
                     int*, int*, int*, int*, double*, double*, double*,
                     double*, double*, double*, int, int, int, int, int, int);
}
/*--------------------------------------------------------------------------*/
static int openMPSFile(types::String* pStrFilename, int* piMode, int* lunit);
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_readmps(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::String* pStrFileName = NULL;

    double dLowBnd  = 0;
    double dUpBnd   = 0;
    int iMaxM       = 0;
    int iMaxN       = 0;
    int iMaxNza     = 0;
    int iM          = 0;
    int iN          = 0;
    int iNza        = 0;
    int lunit       = 0; // file unit. 0 mean we open the file by this name.
    int mlunit      = 0;
    int piMode[2]   = { -1, 0};
    int ierr        = 0;
    int line        = 0;
    char typrow[2];

    wchar_t* wcsFileName = NULL;
    double big = NumericConstants::double_max;

    if (in.size() < 2 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "readmps", 2, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "readmps", 1);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get file name
    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "readmps", 1);
        return types::Function::Error;
    }

    pStrFileName = in[0]->getAs<types::String>();

    if (pStrFileName->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar string expected.\n"), "readmps", 1);
        return types::Function::Error;
    }

    // get Bounds
    if (in[1]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "readmps", 2);
        return types::Function::Error;
    }

    types::Double* pDblBounds = in[1]->getAs<types::Double>();

    if (pDblBounds->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "readmps", 2);
        return types::Function::Error;
    }

    if (pDblBounds->getSize() != 2)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A real vector of size %d expected.\n"), "readmps", 2, 2);
        return types::Function::Error;
    }

    dLowBnd = pDblBounds->get(0);
    dUpBnd  = pDblBounds->get(1);

    if (in.size() == 3)
    {
        // get Max sizes
        if (in[2]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "readmps", 3);
            return types::Function::Error;
        }

        types::Double* pDblMaxSizes = in[2]->getAs<types::Double>();

        if (pDblMaxSizes->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real vector expected.\n"), "readmps", 3);
            return types::Function::Error;
        }

        if (pDblMaxSizes->getSize() != 3)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A real vector of size %d expected.\n"), "readmps", 3, 3);
            return types::Function::Error;
        }

        iMaxM   = (int)pDblMaxSizes->get(0);
        iMaxN   = (int)pDblMaxSizes->get(1);
        iMaxNza = (int)pDblMaxSizes->get(2);
    }
    else
    {
        if (openMPSFile(pStrFileName, piMode, &lunit))
        {
            return types::Function::Error;
        }

        C2F(rdmpsz)(&lunit, &iMaxM, &iMaxN, &iMaxNza, &ierr, typrow, &line);
        mlunit = -lunit;
        C2F(clunit)(&mlunit, NULL, piMode, 0);

        if (ierr == 1)
        {
            Scierror(998, _("%s: Error while reading line %d.\n"), "readmps", line);
            return types::Function::Error;
        }
        else if (ierr == 2)
        {
            Scierror(999, _("%s: Unknown row type %s at line %d.\n"), "readmps", typrow, line);
            return types::Function::Error;
        }

        lunit = 0;
    }

    iM   = iMaxM;
    iN   = iMaxN;
    iNza = iMaxNza;

    /*** read MPS file ***/
    if (openMPSFile(pStrFileName, piMode, &lunit))
    {
        return types::Function::Error;
    }

    // alloc data
    char nameb[9]  = "        ";
    char namec[9]  = "        ";
    char namran[9] = "        ";
    char nambnd[9] = "        ";
    char nammps[9] = "        ";

    char* pstrRwName = new char[8 * iM + 1];
    pstrRwName[8 * iM] = '\0';
    char* pstrClName = new char[8 * iN + 1];
    pstrClName[8 * iN] = '\0';
    char* strErrorBuf = new char[bsiz];

    int irobj       = 0;
    int* piStavar   = new int[iN];
    int* piRwstat   = new int[iM];
    int* piRowcod   = new int[2 * iM];
    int* piColcod   = new int[2 * iN];
    int* piRwnmbs   = new int[iNza];
    int* piClpnts   = new int[iN + 1];
    int* piRow      = new int[iN];

    types::Double* pDblCoef     = new types::Double(iNza, 1);
    types::Double* pDblRhsb     = new types::Double(iM, 1);
    types::Double* pDblRanges   = new types::Double(iM, 1);
    types::Double* pDblBnds     = new types::Double(iN, 2);
    double* pdblRelt            = new double[iN];

    C2F(rdmps1)(&ierr, strErrorBuf, &iMaxM, &iMaxN, &iMaxNza,
                &iM, &iN, &iNza, &irobj, &big, &dLowBnd, &dUpBnd,
                namec, nameb, namran, nambnd, nammps, &lunit,
                pstrRwName, pstrClName,
                piStavar, piRwstat,
                piRowcod, piRowcod + iM,
                piColcod, piColcod + iN,
                piRwnmbs, piClpnts, piRow,
                pDblCoef->get(), pDblRhsb->get(), pDblRanges->get(),
                pDblBnds->get() + iN, pDblBnds->get(), pdblRelt,
                bsiz, 8L, 8L, 8L, 8L, 8L);

    delete[] piRow;
    delete[] pdblRelt;

    mlunit = -lunit;
    C2F(clunit)(&mlunit, NULL, piMode, 0);

    if (ierr)
    {
        int iLen = 4096;
        char* str = strErrorBuf + 4095;
        while (*str == ' ')
        {
            iLen--;
            str--;
        }

        strErrorBuf[iLen] = '\0';
        Scierror(999, "%s", strErrorBuf);
        delete[] strErrorBuf;
        return types::Function::Error;
    }

    delete[] strErrorBuf;
    /*** return output arguments ***/
    types::String* pStr = NULL;
    types::Double* pDbl = NULL;
    double* pdbl        = NULL;

    types::TList* pTL = new types::TList();
    pStr = new types::String(1, 19);
    pStr->set(0,  L"mps");
    pStr->set(1,  L"irobj");
    pStr->set(2,  L"namec");
    pStr->set(3,  L"nameb");
    pStr->set(4,  L"namran");
    pStr->set(5,  L"nambnd");
    pStr->set(6,  L"name");
    pStr->set(7,  L"rownames");
    pStr->set(8,  L"colnames");
    pStr->set(9,  L"rowstat");
    pStr->set(10, L"rowcode");
    pStr->set(11, L"colcode");
    pStr->set(12, L"rownmbs");
    pStr->set(13, L"colpnts");
    pStr->set(14, L"acoeff");
    pStr->set(15, L"rhs");
    pStr->set(16, L"ranges");
    pStr->set(17, L"bounds");
    pStr->set(18, L"stavar");
    pTL->append(pStr);

    pTL->append(new types::Double((double)irobj));
    pTL->append(new types::String(namec));
    pTL->append(new types::String(nameb));
    pTL->append(new types::String(namran));
    pTL->append(new types::String(nambnd));
    pTL->append(new types::String(nammps));

    pStr = new types::String(iM, 1);
    for (int i = 0; i < iM; i++)
    {
        char pstrTemp[9];
        memcpy(pstrTemp, pstrRwName + i * 8, 8);
        pstrTemp[8] = '\0';
        pStr->set(i, pstrTemp);
    }
    pTL->append(pStr);
    delete[] pstrRwName;

    pStr = new types::String(1, iN);
    for (int i = 0; i < iN; i++)
    {
        char pstrTemp[9];
        memcpy(pstrTemp, pstrClName + i * 8, 8);
        pstrTemp[8] = '\0';
        pStr->set(i, pstrTemp);
    }
    pTL->append(pStr);
    delete[] pstrClName;

    pDbl = new types::Double(iM, 1);
    pdbl = pDbl->get();
    for (int i = 0; i < iM; i++)
    {
        pdbl[i] = (double)piRwstat[i];
    }
    pTL->append(pDbl);
    delete[] piRwstat;

    pDbl = new types::Double(iM, 2);
    pdbl = pDbl->get();
    for (int i = 0; i < iM * 2; i++)
    {
        pdbl[i] = (double)piRowcod[i];
    }
    pTL->append(pDbl);
    delete[] piRowcod;

    pDbl = new types::Double(iN, 2);
    pdbl = pDbl->get();
    for (int i = 0; i < iN * 2; i++)
    {
        pdbl[i] = (double)piColcod[i];
    }
    pTL->append(pDbl);
    delete[] piColcod;

    pDbl = new types::Double(iNza, 1);
    pdbl = pDbl->get();
    for (int i = 0; i < iNza; i++)
    {
        pdbl[i] = (double)piRwnmbs[i];
    }
    pTL->append(pDbl);
    delete[] piRwnmbs;

    pDbl = new types::Double(1, iN + 1);
    pdbl = pDbl->get();
    for (int i = 0; i < iN + 1; i++)
    {
        pdbl[i] = (double)piClpnts[i];
    }
    pTL->append(pDbl);
    delete[] piClpnts;

    pTL->append(pDblCoef);
    pTL->append(pDblRhsb);
    pTL->append(pDblRanges);
    pTL->append(pDblBnds);

    pDbl = new types::Double(iN, 1);
    pdbl = pDbl->get();
    for (int i = 0; i < iN; i++)
    {
        pdbl[i] = (double)piStavar[i];
    }
    pTL->append(pDbl);
    delete[] piStavar;

    out.push_back(pTL);
    return types::Function::OK;
}

int openMPSFile(types::String* _pStrFilename, int* _piMode, int* _lunit)
{
    char* pstFilename = wide_string_to_UTF8(_pStrFilename->get(0));
    int iErr = C2F(clunit)(_lunit, pstFilename, _piMode, (int)strlen(pstFilename));
    if (iErr)
    {
        switch (iErr)
        {
            case 65  :
                Scierror(iErr, _("%s: %d logical unit already used.\n"), "readmps", *_lunit);
                break;
            case 66  :
                Scierror(iErr, _("%s: Too many files opened!\n"), "readmps");
                break;
            case 67  :
                Scierror(iErr, _("%s: Unknown file format.\n"), "readmps");
                break;
            case 240 :
                Scierror(iErr, _("%s: File \"%s\" already exists or directory write access denied.\n"), "readmps", pstFilename);
                break;
            case 241 :
                Scierror(iErr, _("%s: File \"%s\" does not exist or read access denied.\n"), "readmps", pstFilename);
                break;
            default  :
                Scierror(iErr, _("%s: Can not open File \"%s\"\n"), "readmps", pstFilename);
        }

        FREE(pstFilename);
        return 1;
    }

    FREE(pstFilename);
    return 0;
}
