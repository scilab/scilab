/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
#include "diary_manager.hxx"
/*--------------------------------------------------------------------------*/
extern "C"
{
#include "gw_output_stream.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "localization.h"
#include "charEncoding.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
}
/*--------------------------------------------------------------------------*/
#define DIARY_SECOND_ARG_LIST L"list"
#define DIARY_SECOND_ARG_CLOSE L"close"
#define DIARY_SECOND_ARG_PAUSE L"pause"
#define DIARY_SECOND_ARG_OFF L"off"
#define DIARY_SECOND_ARG_RESUME L"resume"
#define DIARY_SECOND_ARG_ON L"on"
#define DIARY_SECOND_ARG_NEW L"new"
#define DIARY_SECOND_ARG_APPEND L"append"
#define DIARY_SECOND_ARG_EXISTS L"exists"
#define DIARY_THIRD_ARG_FILTER_COMMAND L"filter=command"
#define DIARY_THIRD_ARG_FILTER_OUTPUT L"filter=output"
#define DIARY_THIRD_ARG_PREFIX_UNIX_EPOCH L"prefix=U"
#define DIARY_THIRD_ARG_PREFIX_DEFAULT L"prefix=YYYY-MM-DD hh:mm:ss"
#define DIARY_THIRD_ARG_PREFIX_ONLY_COMMANDS L"prefix-only-commands"
/*--------------------------------------------------------------------------*/
static int sci_diary_no_rhs(char *fname);
static int sci_diary_one_rhs(char *fname);
static int sci_diary_two_rhs(char *fname);
static int sci_diary_three_rhs(char *fname);
/*--------------------------------------------------------------------------*/
static double *getInputArgumentOneIDs(char *fname,int *sizeReturnedArray, int *ierror);
static wchar_t **getInputArgumentOneFilenames(char *fname,int *sizeReturnedArray, int *ierror);
static wchar_t *getInputArgumentTwo(char *fname, int *ierror);
static wchar_t **getInputArgumentThree(char *fname,int *sizeReturnedArray, int *ierror);
static int checkExistByIDs(char *fname, double *IDs, int size_IDs); 
static int checkExistByFilenames(char *fname, wchar_t **wcFilenames, int size_IDs); 
static int CloseByFilenames(char *fname);
static int CloseByIds(char *fname);
static int PauseByFilenames(char *fname);
static int PauseByIds(char *fname);
static int ResumeByFilenames(char *fname);
static int ResumeByIds(char *fname);
static int ExistByFilenames(char *fname);
static int ExistByIds(char *fname);
static int AppendByFilenames(char *fname, 
							 diary_filter filterMode, 
							 diary_prefix_time_format prefixMode, 
							 diary_prefix_time_filter prefixModeFilter,
							 bool suspended);
static int NewByFilenames(char *fname, 
						  diary_filter filterMode, 
						  diary_prefix_time_format prefixMode, 
						  diary_prefix_time_filter prefixModeFilter,
						  bool suspended);
/*--------------------------------------------------------------------------*/
int sci_diary(char *fname,unsigned long fname_len)
{
	CheckRhs(0,3);
	CheckLhs(0,2);

	switch (Rhs)
	{
	case 0:
		return sci_diary_no_rhs(fname);
	case 1:
		return sci_diary_one_rhs(fname);
	case 2:
		return sci_diary_two_rhs(fname);
	case 3:
		return sci_diary_three_rhs(fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_diary_no_rhs(char *fname)
{
	// [ids, filenames] = diary()
	// [ids, filenames] = diary([],"list")
	StrErr strErr;
	int nb_diary_ids = 0;
	double *diary_ids  = getDiaryIDsAsDouble(&nb_diary_ids);

	if ( (diary_ids) && (nb_diary_ids > 0) )
	{
		strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, nb_diary_ids, 1, diary_ids);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		LhsVar(1) = Rhs + 1;

		delete [] diary_ids;
		diary_ids = NULL;
		nb_diary_ids = 0;
	}
	else
	{
		if (nb_diary_ids == 0)
		{
			strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 0, 0, NULL);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}
			LhsVar(1) = Rhs + 1;

			if (diary_ids) 
			{
				delete [] diary_ids;
				diary_ids = NULL;
				nb_diary_ids = 0;
			}
		}
		else
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
			return 0;
		}
	}

	if (Lhs == 2)
	{
		int nb_diary_filenames = 0;
		wchar_t **wcdiary_filenames = getDiaryFilenames(&nb_diary_filenames);

		if ( (wcdiary_filenames) && (nb_diary_filenames > 0) )
		{
			strErr = createMatrixOfWideString(pvApiCtx, Rhs + 2, nb_diary_filenames, 1, wcdiary_filenames);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			LhsVar(2) = Rhs + 2;

			freeArrayOfWideString(wcdiary_filenames, nb_diary_filenames);
			nb_diary_filenames = 0;
		}
		else
		{
			if (nb_diary_filenames == 0)
			{
				strErr = createMatrixOfDouble(pvApiCtx, Rhs + 2, 0, 0, NULL);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				LhsVar(2) = Rhs + 2;
				if (wcdiary_filenames)
				{
					freeArrayOfWideString(wcdiary_filenames, nb_diary_filenames);
					nb_diary_filenames = 0;
				}
			}
			else
			{
				Scierror(999,_("%s : Memory allocation error.\n"),fname);
				return 0;
			}
		}
	}

	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_diary_one_rhs(char *fname)
{
	StrErr strErr;
	int iType	= 0;
	int *piAddressVarOne = NULL;

	strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return 0;
	}

	strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return 0;
	}

	if (iType == sci_matrix)
	{
		int IDs_size = 0;
		int ierr = 0;
		double * IDs = getInputArgumentOneIDs(fname, &IDs_size, &ierr);
		if (ierr) return 0;

		// diary([])
		// diary(0)
		// close all
		if ( (IDs_size == 0) ||
			 ((IDs_size == 1) && ((int)IDs[0] == 0)))
		{
			diaryCloseAll();
			C2F(putlhsvar)();
		}
		else
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: 0 expected.\n"),fname,1);
		}
	}
	else if (iType == sci_strings)
	{
		int ierr = 0;
		int sizewcFilenames = 0;
		wchar_t ** wcFilenames = getInputArgumentOneFilenames(fname,&sizewcFilenames,&ierr);
		if (ierr) return 0;

		if (sizewcFilenames == 1)
		{
			if (diaryExists(wcFilenames[0]))
			{
				double dID = (double)diaryNew(wcFilenames[0], false);
				if (dID == -1)
				{
					char *utf_str = wide_string_to_UTF8(wcFilenames[0]);
					if (utf_str)
					{
						Scierror(999,_("%s: error can not create diary: %s.\n"), fname, utf_str);
						FREE(utf_str); utf_str = NULL;
					}
					else
					{
						Scierror(999,_("%s: error can not create diary.\n"),fname);
					}
					return 0;
				}

				strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &dID);
				if(strErr.iErr)
				{
						printError(&strErr, 0);
						return 0;
				}

				LhsVar(1) = Rhs + 1;

				if (Lhs == 2)
				{
					wchar_t **wfilenameUsed = new wchar_t*[1];
					wfilenameUsed[0] = getDiaryFilename((int)dID);
					strErr = createMatrixOfWideString(pvApiCtx, Rhs + 2, 1, 1, wfilenameUsed);
					if(strErr.iErr)
					{
							printError(&strErr, 0);
							return 0;
					}

					LhsVar(2) = Rhs + 2;
					FREE(wfilenameUsed[0]);
					delete [] wfilenameUsed;
				}
			}
			else // diary(filename) exists (close diary)
			{
				if (diaryClose(wcFilenames[0]))
				{
					if (wcFilenames)
					{
						if (wcFilenames[0]) {FREE(wcFilenames[0]); wcFilenames[0] = NULL;}
						FREE(wcFilenames); wcFilenames = NULL;
						Scierror(999,_("%s: error can not close diary.\n"),fname);
						return 0;
					}
				}
				LhsVar(1) = 0; 
			}

			freeArrayOfWideString(wcFilenames,sizewcFilenames);
			C2F(putlhsvar)();
		}
		else
		{
			freeArrayOfWideString(wcFilenames,sizewcFilenames);
			Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname, 1);
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname, 1);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_diary_two_rhs(char *fname)
{
	int ierr = 0;
	wchar_t *wcArgumentTwo = getInputArgumentTwo(fname, &ierr);
	StrErr strErr;
	int *piAddressVarOne = NULL;

	strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return 0;
	}

	if (ierr) return 0;

	if (wcArgumentTwo)
	{
		if (wcscmp(wcArgumentTwo, DIARY_SECOND_ARG_LIST) == 0)
		{
			int m1 = 0, n1 = 0;
			strErr = getVarDimension(pvApiCtx, piAddressVarOne,&m1,&n1);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if ( (m1 == n1) && (n1 == 0) )
			{
				// diary() == diary([], "list")
				return sci_diary_no_rhs(fname);
			}
			else
			{
				Scierror(999,_("%s: Wrong value for input argument #%d.\n"),fname, 2);
			}
		}
		else if (wcscmp(wcArgumentTwo, DIARY_SECOND_ARG_CLOSE) == 0)
		{
			int iType = 0;
			strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if(iType == sci_matrix)
			{
				return CloseByIds(fname);
			}
			else if (iType == sci_strings)
			{
				return CloseByFilenames(fname);
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname, 1);
				return 0;
			}
		}
		else if ( (wcscmp(wcArgumentTwo, DIARY_SECOND_ARG_PAUSE) == 0) ||
				  (wcscmp(wcArgumentTwo, DIARY_SECOND_ARG_OFF) == 0) )
		{
			int iType = 0;
			strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if (iType == sci_matrix)
			{
				return PauseByIds(fname);
			}
			else if(iType == sci_strings)
			{
				return PauseByFilenames(fname);
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname, 1);
				return 0;
			}
		}
		else if ( (wcscmp(wcArgumentTwo, DIARY_SECOND_ARG_RESUME) == 0) ||
				  (wcscmp(wcArgumentTwo, DIARY_SECOND_ARG_ON) == 0) )
		{
			int iType = 0;
			strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if (iType == sci_matrix)
			{
				return ResumeByIds(fname);
			}
			else if (iType == sci_strings)
			{
				return ResumeByFilenames(fname);
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname, 1);
				return 0;
			}
		}
		else if (wcscmp(wcArgumentTwo, DIARY_SECOND_ARG_NEW) == 0)
		{
			int iType = 0;
			strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if (iType == sci_strings)
			{
				return NewByFilenames(fname, DIARY_FILTER_INPUT_AND_OUTPUT, 
					PREFIX_TIME_FORMAT_UNIX_EPOCH, 
					PREFIX_FILTER_NONE, false);
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname, 1);
				return 0;
			}
		}
		else if (wcscmp(wcArgumentTwo, DIARY_SECOND_ARG_APPEND) == 0)
		{
			int iType = 0;
			strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if (iType == sci_strings)
			{
				return AppendByFilenames(fname, DIARY_FILTER_INPUT_AND_OUTPUT, 
					PREFIX_TIME_FORMAT_UNIX_EPOCH, 
					PREFIX_FILTER_NONE, false);
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname, 1);
				return 0;
			}
		}
		else if (wcscmp(wcArgumentTwo, DIARY_SECOND_ARG_EXISTS) == 0)
		{
			int iType = 0;
			strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if (iType == sci_matrix)
			{
				return ExistByIds(fname);
			}
			else if (iType == sci_strings)
			{
				return ExistByFilenames(fname);
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname, 1);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong value for input argument #%d.\n"),fname, 2);
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong value for input argument #%d.\n"),fname, 2);
	}
	
	return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_diary_three_rhs(char *fname)
{
	int ierr = 0;
	int size_ArgThree = 0;
	wchar_t **wcArgumentThree = getInputArgumentThree(fname, &size_ArgThree, &ierr);
	if (ierr) return 0;

	if (wcArgumentThree)
	{
		diary_filter filterMode = DIARY_FILTER_INPUT_AND_OUTPUT;
		diary_prefix_time_format iPrefixMode = PREFIX_TIME_FORMAT_UNIX_EPOCH;
		diary_prefix_time_filter iPrefixIoModeFilter = PREFIX_FILTER_NONE;
		bool suspendedDiary = false;

		for (int i = 0; i < size_ArgThree; i++)
		{
			if (wcscmp(wcArgumentThree[i], DIARY_THIRD_ARG_FILTER_COMMAND) == 0)
			{
				filterMode = DIARY_FILTER_ONLY_INPUT; // input only
			}
			else if (wcscmp(wcArgumentThree[i], DIARY_THIRD_ARG_FILTER_OUTPUT) == 0)
			{
				filterMode = DIARY_FILTER_ONLY_OUTPUT; // output only
			}
			else if (wcscmp(wcArgumentThree[i], DIARY_THIRD_ARG_PREFIX_UNIX_EPOCH) == 0)
			{
				iPrefixMode = PREFIX_TIME_FORMAT_UNIX_EPOCH;
				if (iPrefixIoModeFilter == PREFIX_FILTER_NONE) iPrefixIoModeFilter = PREFIX_FILTER_INPUT_AND_OUTPUT;
			}
			else if (wcscmp(wcArgumentThree[i], DIARY_THIRD_ARG_PREFIX_DEFAULT) == 0)
			{
				iPrefixMode = PREFIX_TIME_FORMAT_ISO_8601;
				if (iPrefixIoModeFilter == PREFIX_FILTER_NONE) iPrefixIoModeFilter = PREFIX_FILTER_INPUT_AND_OUTPUT;
			}
			else if (wcscmp(wcArgumentThree[i], DIARY_THIRD_ARG_PREFIX_ONLY_COMMANDS) == 0)
			{
				iPrefixIoModeFilter = PREFIX_FILTER_ONLY_INPUT;
			}
			else if ( (wcscmp(wcArgumentThree[i], DIARY_SECOND_ARG_ON) == 0) || (wcscmp(wcArgumentThree[i], DIARY_SECOND_ARG_RESUME) == 0) )
			{
				suspendedDiary = false;
			}
			else if ( (wcscmp(wcArgumentThree[i], DIARY_SECOND_ARG_OFF) == 0) || (wcscmp(wcArgumentThree[i], DIARY_SECOND_ARG_PAUSE) == 0) )
			{
				suspendedDiary = true;
			}
			else
			{
				freeArrayOfWideString(wcArgumentThree, size_ArgThree);
				Scierror(999,_("%s: Wrong value for input argument #%d.\n"),fname, 3);
				return 0;
			}			
		}
		freeArrayOfWideString(wcArgumentThree, size_ArgThree);

		wchar_t *wcArgumentTwo = getInputArgumentTwo(fname, &ierr);
		if (ierr) return 0;
		if (wcArgumentTwo)
		{
			int *piAddressVarOne = NULL;
			StrErr strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if (wcscmp(wcArgumentTwo, DIARY_SECOND_ARG_NEW) == 0)
			{
				int iType = 0;
				strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				FREE(wcArgumentTwo); wcArgumentTwo = NULL;
				if (iType == sci_strings)
				{
					return NewByFilenames(fname, filterMode, iPrefixMode, iPrefixIoModeFilter, suspendedDiary);
				}
				else
				{
					Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname, 1);
					return 0;
				}
			}
			else if (wcscmp(wcArgumentTwo, DIARY_SECOND_ARG_APPEND) == 0)
			{
				int iType = 0;
				strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				FREE(wcArgumentTwo); wcArgumentTwo = NULL;
				if (iType == sci_strings)
				{
					return AppendByFilenames(fname, filterMode, iPrefixMode, iPrefixIoModeFilter, suspendedDiary);
				}
				else
				{
					Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname, 1);
					return 0;
				}
			}
			else
			{
				FREE(wcArgumentTwo); wcArgumentTwo = NULL;
				Scierror(999,_("%s: Wrong value for input argument #%d.\n"),fname, 2);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s : Memory allocation error.\n"),fname);
		}
	}
	else
	{
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
static double *getInputArgumentOneIDs(char *fname,int *sizeReturnedArray, int *ierror)
{
	double *IDs = NULL;
	*sizeReturnedArray = 0;
	*ierror = 0;

	int m1 = 0, n1 = 0;
	int *piAddressVarOne = NULL;

	StrErr strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	int iType = 0;
	strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if (iType == sci_matrix)
	{
		strErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne,&m1,&n1,&IDs);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (m1 == 1) || (n1 == 1) )
		{
			*sizeReturnedArray = m1 * n1;
		}
		else if ( (m1 == 0) || (n1 == 0) )
		{
			*sizeReturnedArray = 0;
			*ierror = 2;
			return NULL;
		}
		else
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A vector expected.\n"),fname,1);
			*ierror = 1;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"),fname,1);
		*ierror = 1;
	}

	return IDs;
}
/*--------------------------------------------------------------------------*/
static wchar_t **getInputArgumentOneFilenames(char *fname,int *sizeReturnedArray, int *ierror)
{
	wchar_t** wcFilenames = NULL;
	*sizeReturnedArray = 0;
	*ierror = 0;

	int m1 = 0, n1 = 0;
	int *piAddressVarOne = NULL;

	StrErr strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	int iType = 0;
	strErr = getVarType(pvApiCtx, piAddressVarOne, &iType);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if (iType == sci_strings)
	{
		int *lenStVarOne = NULL;

		strErr = getVarDimension(pvApiCtx, piAddressVarOne,&m1,&n1);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if (m1*n1 < 1)
		{
			Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname,1);
			*ierror = 1;
		}
		else if ( (m1 == 1) || (n1 == 1) )
		{
			lenStVarOne = (int *)MALLOC(sizeof(int) * (m1 * n1));
			if (lenStVarOne == NULL)
			{
				Scierror(999,_("%s : Memory allocation error.\n"),fname);
				*ierror = 1;
			}
			else
			{
				strErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, wcFilenames);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				wcFilenames = (wchar_t **)MALLOC(sizeof(wchar_t *) * (m1 * n1));
				if (wcFilenames == NULL)
				{
					Scierror(999,_("%s : Memory allocation error.\n"),fname);
					*ierror = 1;
				}
				else
				{
					strErr = getMatrixOfWideString(pvApiCtx, piAddressVarOne, &m1, &n1, lenStVarOne, wcFilenames);
					if(strErr.iErr)
					{
						printError(&strErr, 0);
						return 0;
					}

					*sizeReturnedArray = m1 * n1;
				}
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A vector expected.\n"),fname,1);
			*ierror = 1;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		*ierror = 1;
	}
	return wcFilenames;
}
/*--------------------------------------------------------------------------*/
static wchar_t *getInputArgumentTwo(char *fname, int *ierror)
{
	wchar_t *wcInputArgumentTwo = NULL;
	*ierror = 0;

	int m2 = 0, n2 = 0;
	int *piAddressVarTwo = NULL;

	StrErr strErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	int iType = 0;
	strErr = getVarType(pvApiCtx, piAddressVarTwo, &iType);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if (iType == sci_strings)
	{
		int lenStVarTwo = 0;

		strErr = getVarDimension(pvApiCtx, piAddressVarTwo,&m2,&n2);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if ( (m2 != n2) && (n2 != 1) ) 
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
			*ierror = 1;
		}
		else
		{
			strErr = getMatrixOfWideString(pvApiCtx, piAddressVarTwo,&m2,&n2,&lenStVarTwo,&wcInputArgumentTwo);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			if (wcInputArgumentTwo == NULL)
			{
				Scierror(999,_("%s : Memory allocation error.\n"),fname);
				*ierror = 1;
			}
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,2);
		*ierror = 1;
	}
	return wcInputArgumentTwo;
}
/*--------------------------------------------------------------------------*/
static wchar_t** getInputArgumentThree(char *fname,int *sizeReturnedArray, int *ierror)
{
	wchar_t** wcInputArgumentThree = NULL;
	*sizeReturnedArray = 0;
	*ierror = 0;

	int m3 = 0, n3 = 0;
	int *piAddressVarThree = NULL;

	StrErr strErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	int iType = 0;
	strErr = getVarType(pvApiCtx, piAddressVarThree, &iType);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	if (iType == sci_strings)
	{
		int *lenStVarThree = NULL;

		strErr = getVarDimension(pvApiCtx, piAddressVarThree,&m3,&n3);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		if (m3*n3 < 1)
		{
			Scierror(999,_("%s: Wrong size for input argument #%d.\n"),fname,3);
			*ierror = 1;
		}
		else if ( (m3 == 1) || (n3 == 1) )
		{
			lenStVarThree = (int *)MALLOC(sizeof(int) * (m3 * n3));
			if (lenStVarThree == NULL)
			{
				Scierror(999,_("%s : Memory allocation error.\n"),fname);
				*ierror = 1;
			}
			else
			{
				strErr = getMatrixOfWideString(pvApiCtx, piAddressVarThree, &m3, &n3, lenStVarThree, wcInputArgumentThree);
				if(strErr.iErr)
				{
					printError(&strErr, 0);
					return 0;
				}

				wcInputArgumentThree = (wchar_t **)MALLOC(sizeof(wchar_t *) * (m3 * n3));
				if (wcInputArgumentThree == NULL)
				{
					Scierror(999,_("%s : Memory allocation error.\n"),fname);
					*ierror = 1;
				}
				else
				{
					strErr = getMatrixOfWideString(pvApiCtx, piAddressVarThree, &m3, &n3, lenStVarThree, wcInputArgumentThree);
					if(strErr.iErr)
					{
						printError(&strErr, 0);
						return 0;
					}

					*sizeReturnedArray = m3 * n3;
				}
			}
		}
		else
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A vector expected.\n"),fname,3);
			*ierror = 1;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,3);
		*ierror = 1;
	}
	return wcInputArgumentThree;
}
/*--------------------------------------------------------------------------*/
static int checkExistByIDs(char *fname, double *IDs, int size_IDs)
{
	if (IDs)
	{
		for (int i = 0; i < size_IDs; i++)
		{
			if (diaryExists((int)IDs[i]))
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: diary ID %d not exists.\n"),fname,1, (int)IDs[i]);
				return 1;
			}
		}
		return 0;
	}
	return 1;
}
/*--------------------------------------------------------------------------*/
static int checkExistByFilenames(char *fname, wchar_t **wcFilenames, int size_IDs)
{
	if (wcFilenames)
	{
		for (int i = 0; i < size_IDs; i++)
		{
			if (diaryExists(wcFilenames[i]))
			{
				Scierror(999,_("%s: Wrong value for input argument #%d: diary filename not exists.\n"),fname,1);
				return 1;
			}
		}
		return 0;
	}
	return 1;
}
/*--------------------------------------------------------------------------*/
static int CloseByFilenames(char *fname)
{
	wchar_t **wcFilenames = NULL;
	int dIDs_size = 0;
	int ierr = 0;

	wcFilenames = getInputArgumentOneFilenames(fname, &dIDs_size, &ierr);
	if (ierr) return 0;

	ierr = checkExistByFilenames(fname, wcFilenames, dIDs_size);
	if (ierr) 
	{
		freeArrayOfWideString(wcFilenames, dIDs_size);
		return 0;
	}

	for (int i = 0; i < dIDs_size; i++)
	{
		if (diaryClose(wcFilenames[i]))
		{
			freeArrayOfWideString(wcFilenames, dIDs_size);
			Scierror(999,_("%s: Wrong value for input argument #%d: error can not close diary.\n"),fname);
			return 0;
		}
	}

	freeArrayOfWideString(wcFilenames, dIDs_size);
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
static int CloseByIds(char *fname)
{
	double *dIDs = NULL;
	int dIDs_size = 0;
	int ierr = 0;

	dIDs = getInputArgumentOneIDs(fname, &dIDs_size, &ierr);

	if (ierr == 2)
	{
		// diary([], 'close')
		diaryCloseAll();
		C2F(putlhsvar)();
		return 0;
	}
	else if (ierr) return 0;

	ierr = checkExistByIDs(fname, dIDs, dIDs_size);
	if (ierr) return 0;

	for (int i = 0; i < dIDs_size; i++)
	{
		if (diaryClose((int)dIDs[i]))
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: error can not close diary %d.\n"),fname, (int)dIDs[i]);
			return 0;
		}
	}
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
static int PauseByFilenames(char *fname)
{
	wchar_t **wcFilenames = NULL;
	int dIDs_size = 0;
	int ierr = 0;

	wcFilenames = getInputArgumentOneFilenames(fname, &dIDs_size, &ierr);
	if (ierr) return 0;

	ierr = checkExistByFilenames(fname, wcFilenames, dIDs_size);
	if (ierr) 
	{
		freeArrayOfWideString(wcFilenames, dIDs_size);
		return 0;
	}

	for (int i = 0; i < dIDs_size; i++)
	{
		if (diaryPause(wcFilenames[i]))
		{
			freeArrayOfWideString(wcFilenames, dIDs_size);
			Scierror(999,_("%s: Wrong value for input argument #%d: error can not pause diary.\n"),fname);
			return 0;
		}
	}

	freeArrayOfWideString(wcFilenames, dIDs_size);

	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
static int PauseByIds(char *fname)
{
	double *dIDs = NULL;
	int dIDs_size = 0;
	int ierr = 0;

	dIDs = getInputArgumentOneIDs(fname, &dIDs_size, &ierr);
	if (ierr) return 0;

	ierr = checkExistByIDs(fname, dIDs, dIDs_size);
	if (ierr) return 0;

	for (int i = 0; i < dIDs_size; i++)
	{
		if (diaryPause((int)dIDs[i]))
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: error can not pause diary %d.\n"),fname, (int)dIDs[i]);
			return 0;
		}
	}

	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
static int ResumeByFilenames(char *fname)
{
	wchar_t **wcFilenames = NULL;
	int dIDs_size = 0;
	int ierr = 0;
	wcFilenames = getInputArgumentOneFilenames(fname, &dIDs_size, &ierr);
	if (ierr) return 0;

	ierr = checkExistByFilenames(fname, wcFilenames, dIDs_size);
	if (ierr) 
	{
		freeArrayOfWideString(wcFilenames, dIDs_size);
		return 0;
	}

	for (int i = 0; i < dIDs_size; i++)
	{
		if (diaryResume(wcFilenames[i]))
		{
			freeArrayOfWideString(wcFilenames, dIDs_size);
			Scierror(999,_("%s: Wrong value for input argument #%d: error can not resume diary.\n"),fname);
			return 0;
		}
	}

	freeArrayOfWideString(wcFilenames, dIDs_size);

	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
static int ResumeByIds(char *fname)
{
	double *dIDs = NULL;
	int dIDs_size = 0;
	int ierr = 0;

	dIDs = getInputArgumentOneIDs(fname, &dIDs_size, &ierr);
	if (ierr) return 0;

	ierr = checkExistByIDs(fname, dIDs, dIDs_size);
	if (ierr) return 0;

	for (int i = 0; i < dIDs_size; i++)
	{
		if (diaryResume((int)dIDs[i]))
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: error can not resume diary %d.\n"),fname, (int)dIDs[i]);
			return 0;
		}
	}

	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
static int ExistByFilenames(char *fname)
{
	wchar_t **wcFilenames = NULL;
	int dIDs_size = 0;
	int ierr = 0;
	wcFilenames = getInputArgumentOneFilenames(fname, &dIDs_size, &ierr);
	if (ierr) return 0;

	int *resultExist = (int*)MALLOC(sizeof(int)*dIDs_size);
	if (resultExist == NULL)
	{
		freeArrayOfWideString(wcFilenames,dIDs_size);
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return 0;
	}
	
	for(int i=0; i < dIDs_size; i++)
	{
		int bExists = diaryExists(wcFilenames[i]);
		if ( bExists == 0)
		{
			resultExist[i]  = TRUE;
		}
		else
		{
			resultExist[i]  = FALSE;
		}
	}
	freeArrayOfWideString(wcFilenames,dIDs_size);

	StrErr strErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, dIDs_size, resultExist);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	FREE(resultExist); resultExist = NULL;
	LhsVar(1) = Rhs + 1;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
static int ExistByIds(char *fname)
{
	double *dIDs = NULL;
	int dIDs_size = 0;
	int ierr = 0;

	dIDs = getInputArgumentOneIDs(fname, &dIDs_size, &ierr);
	if (ierr) return 0;

	int *resultExist = (int*)MALLOC(sizeof(int)*dIDs_size);
	if (resultExist == NULL)
	{
		Scierror(999,_("%s : Memory allocation error.\n"),fname);
		return 0;
	}

	for(int i=0; i < dIDs_size; i++)
	{
		int bExists = diaryExists((int)dIDs[i]);
		if ( bExists == 0)
		{
			resultExist[i]  = TRUE;
		}
		else
		{
			resultExist[i]  = FALSE;
		}
	}

	StrErr strErr = createMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, dIDs_size, resultExist);
	if(strErr.iErr)
	{
		printError(&strErr, 0);
		return 0;
	}

	FREE(resultExist); resultExist = NULL;
	LhsVar(1) = Rhs + 1;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
static int AppendByFilenames(char *fname, 
							 diary_filter filterMode, 
							 diary_prefix_time_format prefixMode, 
							 diary_prefix_time_filter prefixModeFilter,
							 bool suspended)
{
	StrErr strErr;
	wchar_t **wcFilenames = NULL;
	int dIDs_size = 0;
	int ierr = 0;

	wcFilenames = getInputArgumentOneFilenames(fname, &dIDs_size, &ierr);
	if (ierr) return 0;

	if (dIDs_size == 1)
	{
		double dID = (double)diaryAppend(wcFilenames[0]);
		if (dID == -1)
		{
			char *utf_str = wide_string_to_UTF8(wcFilenames[0]);
			if (utf_str)
			{
				Scierror(999,_("%s: error can not create diary: %s.\n"), fname, utf_str);
				FREE(utf_str); utf_str = NULL;
			}
			else
			{
				Scierror(999,_("%s: error can not create diary.\n"),fname);
			}
			freeArrayOfWideString(wcFilenames, dIDs_size);
			return 0;
		}

		freeArrayOfWideString(wcFilenames, dIDs_size);

		diarySetFilterMode((int)dID, filterMode);
		diarySetPrefixMode((int)dID, prefixMode);
		diarySetPrefixIoModeFilter((int)dID, prefixModeFilter);
		if (suspended) diaryPause((int)dID);

		strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &dID);
		if(strErr.iErr)
		{
			printError(&strErr, 0);
			return 0;
		}

		LhsVar(1) = Rhs + 1;

		if (Lhs == 2)
		{
			wchar_t **wfilenameUsed = new wchar_t*[1];
			wfilenameUsed[0] = getDiaryFilename((int)dID);
			strErr = createMatrixOfWideString(pvApiCtx, Rhs + 2, 1, 1, wfilenameUsed);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			LhsVar(2) = Rhs + 2;
			freeArrayOfWideString(wfilenameUsed, 1);
		}
		C2F(putlhsvar)();
	}
	else
	{
		freeArrayOfWideString(wcFilenames, dIDs_size);
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
static int NewByFilenames(char *fname,
						  diary_filter filterMode, 
						  diary_prefix_time_format prefixMode, 
						  diary_prefix_time_filter prefixModeFilter, 
						  bool suspended)
{
	wchar_t **wcFilenames = NULL;
	int dIDs_size = 0;
	int ierr = 0;

	wcFilenames = getInputArgumentOneFilenames(fname, &dIDs_size, &ierr);
	if (ierr) return 0;

	if (dIDs_size == 1)
	{
		double dID = (double)diaryNew(wcFilenames[0], true);
		if (dID == -1)
		{
			char *utf_str = wide_string_to_UTF8(wcFilenames[0]);
			if (utf_str)
			{
				Scierror(999,_("%s: error can not create diary: %s.\n"), fname, utf_str);
				FREE(utf_str); utf_str = NULL;
			}
			else
			{
				Scierror(999,_("%s: error can not create diary.\n"),fname);
			}
			freeArrayOfWideString(wcFilenames, dIDs_size);
			return 0;
		}

		freeArrayOfWideString(wcFilenames, dIDs_size);

		diarySetFilterMode((int)dID, filterMode);
		diarySetPrefixMode((int)dID, prefixMode);
		diarySetPrefixIoModeFilter((int)dID, prefixModeFilter);
		if (suspended) diaryPause((int)dID);

		StrErr strErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, 1, 1, &dID);
		LhsVar(1) = Rhs + 1;

		if (Lhs == 2)
		{
			wchar_t **wfilenameUsed = new wchar_t*[1];
			wfilenameUsed[0] = getDiaryFilename((int)dID);
			strErr = createMatrixOfWideString(pvApiCtx, Rhs + 2, 1, 1, wfilenameUsed);
			if(strErr.iErr)
			{
				printError(&strErr, 0);
				return 0;
			}

			LhsVar(2) = Rhs + 2;
			FREE(wfilenameUsed[0]);
			delete [] wfilenameUsed;
		}
		C2F(putlhsvar)();
	}
	else
	{
		freeArrayOfWideString(wcFilenames, dIDs_size);
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
