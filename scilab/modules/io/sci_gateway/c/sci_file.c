/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "gw_io.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "FileExist.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
#define FILE_OPEN_STR "open"
#define FILE_OLD_STR "old"
/*--------------------------------------------------------------------------*/
int C2F(sci_file)(char *fname,unsigned long fname_len)
{
	if (Rhs >= 3)
	{
		SciErr sciErr;

		int *piAddressVarOne = NULL;
		int *piAddressVarTwo = NULL;
		int *piAddressVarThree = NULL;

		int iType1 = 0;
		int iType2 = 0;
		int iType3 = 0;

		sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		sciErr = getVarType(pvApiCtx, piAddressVarThree, &iType3);
		if(sciErr.iErr)
		{
			printError(&sciErr, 0);
			return 0;
		}

		if ( (iType1 == sci_strings) && (iType2 == sci_strings) && (iType3 == sci_strings) )
		{
			char *pStVarOne = NULL;
			int lenStVarOne = 0;

			wchar_t *pStVarTwo = NULL;
			int lenStVarTwo = 0;

			char *pStVarThree = NULL;
			int lenStVarThree = 0;

			int m1 = 0, n1 = 0;
			int m2 = 0, n2 = 0;
			int m3 = 0, n3 = 0;

			sciErr = getVarDimension(pvApiCtx, piAddressVarOne, &m1, &n1);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			if ( (m1 != n1) && (n1 != 1) ) 
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 1);
				return 0;
			}

			sciErr = getVarDimension(pvApiCtx, piAddressVarTwo, &m2, &n2);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			if ( (m2 != n2) && (n2 != 1) ) 
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 2);
				return 0;
			}

			sciErr = getVarDimension(pvApiCtx, piAddressVarThree, &m3, &n3);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			if ( (m3 != n3) && (n3 != 1) ) 
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), fname, 3);
				return 0;
			}

			// get lenStVarOne value
			sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
			if(sciErr.iErr)
			{
				printError(&sciErr, 0);
				return 0;
			}

			// get value of first argument
			pStVarOne = (char*)MALLOC(sizeof(char)*(lenStVarOne + 1));
			if (pStVarOne == NULL)
			{
				Scierror(999,_("%s : Memory allocation error.\n"),fname);
				return 0;
			}

			sciErr = getMatrixOfString(pvApiCtx, piAddressVarOne, &m1, &n1, &lenStVarOne, &pStVarOne);
			if(sciErr.iErr)
			{
				FREE(pStVarOne); pStVarOne = NULL;
				printError(&sciErr, 0);
				return 0;
			}

			if (strcmp(pStVarOne, FILE_OPEN_STR) == 0)
			{
				FREE(pStVarOne); pStVarOne = NULL;

				// get lenStVarThree value
				sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &m3, &n3, &lenStVarThree, &pStVarThree);
				if(sciErr.iErr)
				{
					printError(&sciErr, 0);
					return 0;
				}

				// get value of third argument
				pStVarThree = (char*)MALLOC(sizeof(char)*(lenStVarThree + 1));
				if (pStVarThree == NULL)
				{
					Scierror(999,_("%s : Memory allocation error.\n"),fname);
					return 0;
				}

				sciErr = getMatrixOfString(pvApiCtx, piAddressVarThree, &m3, &n3, &lenStVarThree, &pStVarThree);
				if(sciErr.iErr)
				{
					FREE(pStVarThree); pStVarThree = NULL; 
					printError(&sciErr, 0);
					return 0;
				}

				if (strcmp(pStVarThree, FILE_OLD_STR) == 0)
				{
					FREE(pStVarThree); pStVarThree = NULL; 

					// get lenStVarTwo value
					sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &pStVarTwo);
					if(sciErr.iErr)
					{
						printError(&sciErr, 0);
						return 0;
					}

					// get value of second argument
					pStVarTwo = (wchar_t*)MALLOC(sizeof(wchar_t)*(lenStVarTwo + 1));
					if (pStVarTwo == NULL)
					{
						Scierror(999,_("%s : Memory allocation error.\n"),fname);
						return 0;
					}

					sciErr = getMatrixOfWideString(pvApiCtx, piAddressVarTwo, &m2, &n2, &lenStVarTwo, &pStVarTwo);
					if(sciErr.iErr)
					{
						FREE(pStVarTwo); pStVarTwo = NULL;
						printError(&sciErr, 0);
						return 0;
					}

					if (!FileExistW(pStVarTwo))
					{
						if (Lhs == 2)
						{
							double dOutErrCode = 240.;
							int m_out = 1,  n_out = 1;

							sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, NULL);
							if(sciErr.iErr)
							{
								printError(&sciErr, 0);
								return 0;
							}

							sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 2, m_out, n_out, &dOutErrCode);
							if(sciErr.iErr)
							{
								printError(&sciErr, 0);
								return 0;
							}

							LhsVar(2) = Rhs + 2;
							LhsVar(1) = Rhs + 1;
							C2F(putlhsvar)();
						}
						else
						{
							char *filename = wide_string_to_UTF8(pStVarTwo);
							if (filename)
							{
								Scierror(240, _("%s: The file \"%s\" does not exist.\n"),fname, filename);
								FREE(filename);
								filename = NULL;
							}
							else
							{
								Scierror(240, _("%s: The file does not exist.\n"),fname);
							}
						}
						return 0;
					}

					FREE(pStVarTwo); pStVarTwo = NULL;
				}

				FREE(pStVarThree); pStVarThree = NULL; 
			}

			FREE(pStVarOne); pStVarOne = NULL;
		}
	}

	C2F(intfile)();
	return 0;
}
/*--------------------------------------------------------------------------*/
