/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009-2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "funcmanager.hxx"
#include "context.hxx"
#include "io_gw.hxx"
#include "setenvvar.hxx"
#include "filemanager.hxx"
#include "string.hxx"

extern "C"
{
#include "os_wcsdup.h"
#include "localization.h"
#include "Scierror.h"
}
/*
#include <string.h>
#include "gw_io.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "FileExist.h"
#include "MALLOC.h"
#include "charEncoding.h"
#include "filesmanagement.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
*/
/*--------------------------------------------------------------------------*/
#define FILE_OPEN_STR "open"
#define FILE_OLD_STR "old"
/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_file_no_rhs(types::typed_list &in, int _iRetCount, types::typed_list &out);
Function::ReturnValue sci_file_one_rhs(types::typed_list &in, int _iRetCount, types::typed_list &out);
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_file(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() == 0)
    {
        return sci_file_no_rhs(in, _iRetCount, out);
    }

    if (in.size() == 1)
    {
        return sci_file_one_rhs(in, _iRetCount, out);
    }
    return Function::OK;
}

//int sci_file(char *fname, void* pvApiCtx)
//{
//	if (Rhs == 0)
//	{
//		return sci_file_no_rhs(fname, pvApiCtx);
//	}
//
//	if (Rhs == 1)
//	{
//		return sci_file_one_rhs(fname, pvApiCtx);
//	}
//
//	if (Rhs >= 3)
//	{
//		SciErr sciErr;
//
//		int *piAddressVarOne = NULL;
//		int *piAddressVarTwo = NULL;
//		int *piAddressVarThree = NULL;
//
//		int iType1 = 0;
//		int iType2 = 0;
//		int iType3 = 0;
//
//		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
//		if(sciErr.iErr)
//		{
//			printError(&sciErr, 0);
//			return 0;
//		}
//
//		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
//		if(sciErr.iErr)
//		{
//			printError(&sciErr, 0);
//			return 0;
//		}
//
//		sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
//		if(sciErr.iErr)
//		{
//			printError(&sciErr, 0);
//			return 0;
//		}
//
//		sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
//		if(sciErr.iErr)
//		{
//			printError(&sciErr, 0);
//			return 0;
//		}
//
//		sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
//		if(sciErr.iErr)
//		{
//			printError(&sciErr, 0);
//			return 0;
//		}
//
//		sciErr = getVarType(pvApiCtx, piAddressVarThree, &iType3);
//		if(sciErr.iErr)
//		{
//			printError(&sciErr, 0);
//			return 0;
//		}
//
//		if ( (iType1 == sci_strings) && (iType2 == sci_strings) && (iType3 == sci_strings) )
//		{
//			char *pStVarOne = NULL;
//			int lenStVarOne = 0;
//
//			wchar_t *pStVarTwo = NULL;
//			int lenStVarTwo = 0;
//
//			char *pStVarThree = NULL;
//			int lenStVarThree = 0;
//
//			int m1 = 0, n1 = 0;
//			int m2 = 0, n2 = 0;
//			int m3 = 0, n3 = 0;
//
//			sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
//			if(sciErr.iErr)
//			{
//				printError(&sciErr, 0);
//				return 0;
//			}
//
//			if ( (m1 != n1) && (n1 != 1) )
//			{
//				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
//				return 0;
//			}
//
//			sciErr = getVarDimension(pvApiCtx, piAddressVarTwo, &m2, &n2);
//			if(sciErr.iErr)
//			{
//				printError(&sciErr, 0);
//				return 0;
//			}
//
//			if ( (m2 != n2) && (n2 != 1) )
//			{
//				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
//				return 0;
//			}
//
//			sciErr = getVarDimension(pvApiCtx, piAddressVarThree, &m3, &n3);
//			if(sciErr.iErr)
//			{
//				printError(&sciErr, 0);
//				return 0;
//			}
//
//			if ( (m3 != n3) && (n3 != 1) )
//			{
//				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 3);
//				return 0;
//			}
//
//			// get lenStVarOne value
//			sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
//			if(sciErr.iErr)
//			{
//				printError(&sciErr, 0);
//				return 0;
//			}
//
//			// get value of first argument
//			pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));
//			if (pStVarOne == NULL)
//			{
//				Scierror(999,_("%s: Memory allocation error.\n"),fname);
//				return 0;
//			}
//
//			sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
//			if(sciErr.iErr)
//			{
//				FREE(pStVarOne); pStVarOne = NULL;
//				printError(&sciErr, 0);
//				return 0;
//			}
//
//			if (strcmp(pStVarOne, FILE_OPEN_STR) == 0)
//			{
//				FREE(pStVarOne); pStVarOne = NULL;
//
//				// get lenStVarThree value
//				sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &m3, &n3, &lenStVarThree, &pStVarThree);
//				if(sciErr.iErr)
//				{
//					printError(&sciErr, 0);
//					return 0;
//				}
//
//				// get value of third argument
//				pStVarThree = (char*)MALLOC(sizeof(char)*(lenStVarThree + 1));
//				if (pStVarThree == NULL)
//				{
//					Scierror(999,_("%s: Memory allocation error.\n"),fname);
//					return 0;
//				}
//
//				sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &m3, &n3, &lenStVarThree, &pStVarThree);
//				if(sciErr.iErr)
//				{
//					FREE(pStVarThree); pStVarThree = NULL;
//					printError(&sciErr, 0);
//					return 0;
//				}
//
//				if (strcmp(pStVarThree, FILE_OLD_STR) == 0)
//				{
//					FREE(pStVarThree); pStVarThree = NULL;
//
//					// get lenStVarTwo value
//					sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &pStVarTwo);
//					if(sciErr.iErr)
//					{
//						printError(&sciErr, 0);
//						return 0;
//					}
//
//					// get value of second argument
//					pStVarTwo = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarTwo + 1));
//					if (pStVarTwo == NULL)
//					{
//						Scierror(999,_("%s: Memory allocation error.\n"),fname);
//						return 0;
//					}
//
//					sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &pStVarTwo);
//					if(sciErr.iErr)
//					{
//						FREE(pStVarTwo); pStVarTwo = NULL;
//						printError(&sciErr, 0);
//						return 0;
//					}
//
//					if (!FileExistW(pStVarTwo))
//					{
//						if (Lhs == 2)
//						{
//							double dOutErrCode = 240.;
//							int m_out = 1,  n_out = 1;
//
//							sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, NULL);
//							if(sciErr.iErr)
//							{
//								printError(&sciErr, 0);
//								return 0;
//							}
//
//							sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 2, m_out, n_out, &dOutErrCode);
//							if(sciErr.iErr)
//							{
//								printError(&sciErr, 0);
//								return 0;
//							}
//
//							LhsVar(2) = Rhs + 2;
//							LhsVar(1) = Rhs + 1;
//							C2F(putlhsvar)();
//						}
//						else
//						{
//							char *filename = wide_string_to_UTF8(pStVarTwo);
//							if (filename)
//							{
//								Scierror(240, _("%s: The file \"%s\" does not exist.\n"),fname, filename);
//								FREE(filename);
//								filename = NULL;
//							}
//							else
//							{
//								Scierror(240, _("%s: The file does not exist.\n"),fname);
//							}
//						}
//						return 0;
//					}
//
//					FREE(pStVarTwo); pStVarTwo = NULL;
//				}
//
//				FREE(pStVarThree); pStVarThree = NULL;
//			}
//
//			FREE(pStVarOne); pStVarOne = NULL;
//		}
//	}
//
//	C2F(intfile)();
//	return 0;
//}
///*--------------------------------------------------------------------------*/
Function::ReturnValue sci_file_no_rhs(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iCount = FileManager::getOpenedCount();
    if (iCount == 0)
    {
        for (int i = 0 ; i < _iRetCount ; i++)
        {
            out.push_back(Double::Empty());
        }
        return Function::OK;
    }

    int* piIds = FileManager::getIDs();
    if (piIds)
    {
        Double *pD = new Double(1, iCount);
        pD->setInt(piIds);
        out.push_back(pD);
        delete[] piIds;
    }

    if (_iRetCount > 1) /*types*/
    {
        wchar_t** pstTypes = FileManager::getTypesAsString();
        if (pstTypes != NULL)
        {
            String* pS = new String(1, iCount);
            pS->set(pstTypes);
            out.push_back(pS);
            for (int i = 0 ; i < iCount ; i++)
            {
                delete[] pstTypes[i];
            }
            delete[] pstTypes;
        }
    }

    if (_iRetCount > 2) /*names*/
    {
        wchar_t** pstNames = FileManager::getFilenames();
        if (pstNames != NULL)
        {
            String* pS = new String(1, iCount);
            pS->set(pstNames);
            out.push_back(pS);
            for (int i = 0 ; i < iCount ; i++)
            {
                delete[] pstNames[i];
            }
            delete[] pstNames;
        }
    }

    if (_iRetCount > 3) /* mod */
    {
        double* pdblModes = FileManager::getModes();
        if (pdblModes != NULL)
        {
            Double* pD = new Double(1, iCount);
            pD->set(pdblModes);
            out.push_back(pD);
            delete[] pdblModes;
        }
    }

    if (_iRetCount > 4) /* swap */
    {
        double* pdblSwaps = FileManager::getSwaps();
        if (pdblSwaps != NULL)
        {
            Double* pD = new Double(1, iCount);
            pD->set(pdblSwaps);
            out.push_back(pD);
            delete[] pdblSwaps;
        }
    }

    return Function::OK;
}
/*--------------------------------------------------------------------------*/
Function::ReturnValue sci_file_one_rhs(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in[0]->isDouble() == false || in[0]->getAs<Double>()->getSize() != 1)
    {
        Scierror(201, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "file", 1);
        return Function::Error;
    }

    Double* pD = in[0]->getAs<Double>();
    int iID = static_cast<int>(pD->getReal()[0]);

    //check if double value is an integer to exclude decimal values
    if (static_cast<double>(iID) != pD->getReal()[0])
    {
        Scierror(201, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "file", 1);
        return Function::Error;
    }

    File *pF = FileManager::getFile(iID);
    if (pF == NULL)
    {
        for (int i = 0 ; i < _iRetCount ; i++)
        {
            out.push_back(Double::Empty());
        }
        return Function::OK;
    }

    out.push_back(new Double(iID));
    if (_iRetCount > 1) /*type*/
    {
        wchar_t* pstType = os_wcsdup(pF->getFileTypeAsString().c_str());
        if (pstType != NULL)
        {
            String* pS = new String(pstType);
            out.push_back(pS);
            delete[] pstType;
        }
    }

    if (_iRetCount > 2) /*name*/
    {
        wchar_t* pstName =  os_wcsdup(pF->getFilename().c_str());
        if (pstName != NULL)
        {
            String* pS = new String(pstName);
            out.push_back(pS);
            delete[] pstName;
        }
    }

    if (_iRetCount > 3) /* mod */
    {
        out.push_back(new Double(pF->getFileModeAsDouble()));
    }

    if (_iRetCount > 4) /* swap */
    {
        out.push_back(new Double(pF->getFileSwap()));
    }
    return Function::OK;
}
//static int sci_file_one_rhs(char *fname, void* pvApiCtx)
//{
//	SciErr sciErr;
//
//	int iID = 0;
//	int m1 = 0, n1 = 0;
//	int iType = 0;
//	int *piAddressVarOne = NULL;
//	double *pdVarOne = NULL;
//
//	int m_out = 0;
//	int n_out = 0;
//
//	/* get Address of inputs */
//	sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
//	if(sciErr.iErr)
//	{
//		printError(&sciErr, 0);
//		return 0;
//	}
//
//	sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
//	if(sciErr.iErr)
//	{
//		printError(&sciErr, 0);
//		return 0;
//	}
//
//	/* check input type */
//	if ( iType != sci_matrix )
//	{
//		Scierror(201,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname,1);
//		return 0;
//	}
//
//	sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne,&m1,&n1,&pdVarOne);
//	if(sciErr.iErr)
//	{
//		printError(&sciErr, 0);
//		return 0;
//	}
//
//	if( n1 != 1 || m1 != 1)
//	{
//		Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
//		return 0;
//	}
//
//	iID = (int) *pdVarOne;
//
//	if (*pdVarOne != (double)iID)
//	{
//		Scierror(999,_("%s: Wrong value for input argument #%d: A integer expected.\n"),fname,1);
//		return 0;
//	}
//
//	/* Lhs = 0 ID */
//	if (GetFileTypeOpenedInScilab(iID) != 0)
//	{
//		m_out = 1;
//		n_out = 1;
//		sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, pdVarOne);
//	}
//	else
//	{
//		/* returns [] */
//		m_out = 0;
//		n_out = 0;
//		sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m_out, n_out, NULL);
//	}
//
//	if(sciErr.iErr)
//	{
//		printError(&sciErr, 0);
//		return 0;
//	}
//
//	LhsVar(1) = Rhs + 1;
//
//	if (Lhs > 1) /* Type */
//	{
//		if (GetFileTypeOpenedInScilab(iID) != 0)
//		{
//			char *TypeIdAsString = GetFileTypeOpenedInScilabAsString(iID);
//			m_out = 1;
//			n_out = 1;
//			sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, m_out, n_out, &TypeIdAsString);
//			FREE(TypeIdAsString);
//			TypeIdAsString = NULL;
//		}
//		else
//		{
//			/* returns [] */
//			m_out = 0;
//			n_out = 0;
//			sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 2, m_out, n_out, NULL);
//		}
//
//		if(sciErr.iErr)
//		{
//			printError(&sciErr, 0);
//			return 0;
//		}
//
//		LhsVar(2) = Rhs + 2;
//	}
//
//	if (Lhs > 2) /* name */
//	{
//		if (GetFileTypeOpenedInScilab(iID) != 0)
//		{
//			char *filename = NULL;
//			m_out = 1;
//			n_out = 1;
//			if (GetFileNameOpenedInScilab(iID) == NULL)
//			{
//				filename = os_strdup("");
//			}
//			else
//			{
//				filename = os_strdup(GetFileNameOpenedInScilab(iID));
//			}
//
//			sciErr = createMatrixOfString(pvApiCtx, Rhs + 3, m_out, n_out, &filename);
//			FREE(filename);
//			filename = NULL;
//		}
//		else
//		{
//			/* returns [] */
//			m_out = 0;
//			n_out = 0;
//			sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 3, m_out, n_out, NULL);
//		}
//
//		if(sciErr.iErr)
//		{
//			printError(&sciErr, 0);
//			return 0;
//		}
//
//		LhsVar(3) = Rhs + 3;
//	}
//
//	if (Lhs > 3)  /* mod */
//	{
//		if (GetFileTypeOpenedInScilab(iID) != 0)
//		{
//			double ModeId = (double)GetFileModeOpenedInScilab(iID);
//			m_out = 1;
//			n_out = 1;
//			sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 4, m_out, n_out, &ModeId);
//		}
//		else
//		{
//			/* returns [] */
//			m_out = 0;
//			n_out = 0;
//			sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 4, m_out, n_out, NULL);
//		}
//
//		if(sciErr.iErr)
//		{
//			printError(&sciErr, 0);
//			return 0;
//		}
//
//		LhsVar(4) = Rhs + 4;
//	}
//
//	if (Lhs > 4) /* swap */
//	{
//		if (GetFileTypeOpenedInScilab(iID) != 0)
//		{
//			double SwapId = (double)GetSwapStatus(iID);
//			m_out = 1;
//			n_out = 1;
//			sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 5, m_out, n_out, &SwapId);
//		}
//		else
//		{
//			/* returns [] */
//			m_out = 0;
//			n_out = 0;
//			sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 5, m_out, n_out, NULL);
//		}
//
//		if(sciErr.iErr)
//		{
//			printError(&sciErr, 0);
//			return 0;
//		}
//		LhsVar(5) = Rhs + 5;
//	}
//
//	C2F(putlhsvar)();
//	return 0;
//}
/*--------------------------------------------------------------------------*/
