/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Cong WU
* Copyright (C) 2008 - 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/* desc : search position of a character string in an other string
using regular expression .                                      */
/*------------------------------------------------------------------------*/
#include "function.hxx"
#include "context.hxx"
#include "types.hxx"
#include "string_gw.hxx"

extern "C"
{
#include "localization.h"
#include "pcre.h"
#include "pcre_private.h"
#include "pcre_error.h"
#include "Scierror.h"
#include "charEncoding.h"
#include "os_strdup.h"
}
/*------------------------------------------------------------------------*/
#define WCHAR_S L's'
#define WCHAR_R L'r'
#define WSTR_ONCE L'o'
/*------------------------------------------------------------------------*/
Function::ReturnValue sci_regexp(typed_list &in, int _iRetCount, typed_list &out)
{
	wchar_t wcType          = WCHAR_S;
    wchar_t* pwstInput      = NULL;
    wchar_t* pwstPattern    = NULL;

    char* pstInput          = NULL;
    char* pstPattern        = NULL;

    int iPcreStatus         = 0;
    int iStart              = 0;
    int iStep               = 0;
    int iEnd                = 0;
    int* piStart            = NULL;
    int* piEnd              = NULL;
    int iOccurs             = 0;

    if(in.size() < 2 || in.size() > 3)
    {
        ScierrorW(999,_W("%ls: Wrong number of input arguments: %d or %d expected.\n"), L"regexp", 2, 3);
        return Function::Error;
    }

    // check output parameters
    if(_iRetCount < 1 || _iRetCount > 3)
    {
        ScierrorW(999,_W("%ls: Wrong number of output arguments: %d expected.\n"), L"regexp", 1);
        return Function::Error;
    }

    if(in[0]->isString() == false || in[0]->getAsString()->size_get() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Single string expected.\n"), L"regexp", 1);
        return Function::Error;
    }
    pwstInput = in[0]->getAsString()->string_get(0);

    if(in[1]->isString() == false || in[1]->getAsString()->size_get() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Single string expected.\n"), L"regexp", 2);
        return Function::Error;
    }
    pwstPattern = in[1]->getAsString()->string_get(0);

    if(in.size() == 3)
    {
        if(in[2]->isString() == false || in[2]->getAsString()->size_get() != 1)
        {
            ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Single string expected.\n"), L"regexp", 3);
            return Function::Error;
        }

        if(in[2]->getAsString()->string_get(0)[0] != WSTR_ONCE)
        {
           ScierrorW(999,_W("%ls: Wrong type for input argument #%d: '%ls' expected.\n"), L"regexp", 3, L"o");
           return Function::Error;
        }
        wcType = WSTR_ONCE;
    }

    pstInput    = wide_string_to_UTF8(pwstInput);
    pstPattern  = wide_string_to_UTF8(pwstPattern);

    //input is empty
    if(wcslen(pwstInput) == 0)
    {
        Double* pStart = new Double(0,0);
        out.push_back(pStart);
        if(_iRetCount > 1)
        {
            Double* pEnd = new Double(0,0);
            out.push_back(pEnd);

            if(_iRetCount > 2)
            {
                String* pS = new String(0,0);
                out.push_back(pS);
            }
        }
        return Function::OK;
    }

    piStart     = new int[wcslen(pwstInput)];
    piEnd       = new int[wcslen(pwstInput)];

    do
    {
        iPcreStatus = pcre_private(pstInput + iStep, pstPattern, &iStart, &iEnd);
        if(iPcreStatus == PCRE_FINISHED_OK && iStart != iEnd)
        {
            char* pstTempStart  = os_strdup(pstInput);
            pstTempStart[iStart + iStep] = 0;

            char* pstTempEnd    = os_strdup(pstInput);
            pstTempEnd[iEnd + iStep]     = 0;

            wchar_t* pwstTempStart  = to_wide_string(pstTempStart);
            wchar_t* pwstTempEnd    = to_wide_string(pstTempEnd);


            piStart[iOccurs] = static_cast<int>(wcslen(pwstTempStart));
            piEnd[iOccurs]   = static_cast<int>(wcslen(pwstTempEnd));

            FREE(pstTempStart);
            FREE(pstTempEnd);
            FREE(pwstTempStart);
            FREE(pwstTempEnd);
            //piStart[iOccurs] = iStart + iStep;
            //piEnd[iOccurs]   = iEnd + iStep;
            iOccurs++;
            iStep += iEnd;
        }
    }while(iPcreStatus == PCRE_FINISHED_OK && iStart != iEnd && wcType != WSTR_ONCE);
 
    FREE(pstInput);
    FREE(pstPattern);

    Double* pStart = new Double(1, iOccurs);
    double* pdblStart = pStart->real_get();

    for(int i = 0 ; i < iOccurs ; i++)
    {
        pdblStart[i] = piStart[i] + 1; //one indexed
    }

    out.push_back(pStart);

    if(_iRetCount > 1)
    {
        Double* pEnd = new Double(1, iOccurs);
        double* pdblEnd = pEnd->real_get();
        for(int i = 0 ; i < iOccurs ; i++)
        {
            pdblEnd[i]   = piEnd[i];
        }
        out.push_back(pEnd);
    }

    if(_iRetCount == 3)
    {
        String *pS = new String(iOccurs, 1);
        for(int i = 0 ; i < iOccurs ; i++)
        {
            wchar_t* pwstTemp = new wchar_t[piEnd[i] - piStart[i] + 1];
            wcsncpy(pwstTemp, pwstInput + piStart[i], piEnd[i] - piStart[i]);
            pwstTemp[piEnd[i] - piStart[i]] = 0;
            pS->string_set(i, 0, pwstTemp);
            delete[] pwstTemp;
        }
        out.push_back(pS);
    }
    delete[] piStart;
    delete[] piEnd;
    return Function::OK;
	//char typ = CHAR_S;
	//char **Str = NULL;
	//char **Str2 = NULL;
	//int i = 0; /* loop indice */

	//int mn = 0; /* dimension parameter 1 m*n */

	//int m2 = 0,n2 = 0;
	//int mn2 = 0; /* m2*n2 */

	//int outIndex = 0;
	//int numRow = 1;

	//int *values_start = NULL;
	//int *values_end = NULL;

	//int *wcvalues_start = NULL;
	//int *wcvalues_end = NULL;


	//int nbValues_start = 0;
	//int nbValues_end=0;
	//int nbposition = 0;

	//CheckRhs(2,3);
	//CheckLhs(1,3);

	//if (VarType(1) == sci_strings)
	//{
	//	int m1 = 0;
	//	int n1 = 0;

	//	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);
	//	mn = m1*n1;
	//}
	//else
	//{
	//	ScierrorW(999, _W("%ls: Wrong type for input argument #%d: Single string expected.\n"), fname,1);
	//	return 0;
	//}

	//if (mn != 1)
	//{
	//	freeArrayOfString(Str,mn);
	//	ScierrorW(36, _W("%ls: Wrong size for input argument #%d: Single string expected.\n"), fname,1);
	//	return 0;
	//}

	//if (VarType(2) == sci_strings)
	//{
	//	int m2 = 0,n2 = 0;
	//	GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Str2);
	//	mn2 = m2*n2;
	//}
	//else
	//{
	//	freeArrayOfString(Str,mn);
	//	ScierrorW(36, _W("%ls: Wrong type for input argument #%d: Single string expected.\n"), fname,2);
	//	return 0;
	//}

	//if (mn2 != 1)
	//{
	//	freeArrayOfString(Str2,mn2);
	//	ScierrorW(36, _W("%ls: Wrong size for input argument #%d: Single string expected.\n"), fname,2);
	//	return 0;
	//}

	//if ( (int)strlen(Str[0]) == 0 )
	//{
	//	values_start = (int *)MALLOC(sizeof(int));
	//	values_end = (int *)MALLOC(sizeof(int));

	//	wcvalues_start = (int *)MALLOC(sizeof(int));
	//	wcvalues_end = (int *)MALLOC(sizeof(int));
	//}
	//else
	//{
	//	values_start = (int *)MALLOC( sizeof(int) * ( strlen(Str[0]) ) );
	//	values_end = (int *)MALLOC( sizeof(int) * ( strlen(Str[0]) ) );

	//	wcvalues_start = (int *)MALLOC( sizeof(int) * ( strlen(Str[0]) ) );
	//	wcvalues_end = (int *)MALLOC( sizeof(int) * ( strlen(Str[0]) ) );
	//}

	//if (Rhs == 2 )
	//{
	//	typ = CHAR_S;
	//}
	//else /* Rhs == 3 */
	//{
	//	int m3 = 0, n3 = 0, l3 = 0;

	//	GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);

	//	if ( m3*n3 != 0) typ = cstk(l3)[0];
	//	if (typ != STR_ONCE)
	//	{
	//		freeArrayOfString(Str,mn);
	//		freeArrayOfString(Str2,mn2);
	//		ScierrorW(999,_W("%ls: Wrong type for input argument #%d: '%s' expected.\n"),fname,3,"o");
	//		return 0;
	//	}
	//}

	//{
	//	int x = 0;
	//	pcre_error_code answer = PCRE_FINISHED_OK;

	//	int start_point = 0;
	//	int Output_Start = 0;
	//	int Output_End = 0;

	//	int wcOutput_Start = 0;
	//	int wcstart_point = 0;
	//	int wcOutput_End = 0;

	//	for (x = 0; x < mn2; ++x)
	//	{
	//		char *save = os_strdup(Str2[x]);
	//		if (save)
	//		{
	//			char *pointer = Str[0];
	//			start_point = 0;
	//			wcstart_point = 0;
	//			do
	//			{
	//				strcpy(save,Str2[x]);
	//				Output_Start = 0;
	//				Output_End = 0;

	//				answer = pcre_private(pointer,save,&Output_Start,&Output_End);
	//				if ( answer == PCRE_FINISHED_OK )
	//				{
	//					/* Start = End means that we matched a position and 0 characters.
	//					* Matching 0 characters, for us, means no match.
	//					*/
	//					if (Output_Start != Output_End)
	//					{
	//						char *	strOutput_Start = os_strdup(pointer);
	//						char *  strOutput_End =  os_strdup(pointer);

	//						wchar_t *wcstrOutput_Start = NULL;
	//						wchar_t *wcstrOutput_End = NULL;

	//						/* calculates positions with wide characters */
	//						strOutput_Start[Output_Start] = '\0';
	//						strOutput_End[Output_End] = '\0';

	//						wcstrOutput_Start = to_wide_string(strOutput_Start);
	//						wcstrOutput_End = to_wide_string(strOutput_End);

	//						if (wcstrOutput_Start)
	//						{
	//							wcOutput_Start = (int)wcslen(wcstrOutput_Start);
	//							FREE(wcstrOutput_Start);wcstrOutput_Start = NULL;
	//						}
	//						else
	//						{
	//							wcOutput_Start = 0;
	//						}

	//						if (wcstrOutput_End)
	//						{
	//							wcOutput_End = (int)wcslen(wcstrOutput_End);
	//							FREE(wcstrOutput_End);wcstrOutput_End = NULL;
	//						}
	//						else
	//						{
	//							wcOutput_End = 0;
	//						}

	//						if (strOutput_Start) {FREE(strOutput_Start); strOutput_Start = NULL;}
	//						if (strOutput_End) {FREE(strOutput_End); strOutput_End = NULL;}

	//						/*adding the answer into the outputmatrix*/
	//						values_start[nbValues_start] = Output_Start + start_point + 1;
	//						values_end[nbValues_end] = Output_End + start_point;

	//						wcvalues_start[nbValues_start] = wcOutput_Start + wcstart_point + 1;
	//						wcvalues_end[nbValues_end] = wcOutput_End + wcstart_point;

	//						nbValues_start++;
	//						nbValues_end++;

	//						/*The number according to the str2 matrix*/
	//						nbposition++;
	//					}
	//					else if(Output_End == 0 && *pointer != '\0')
	//					{
	//						/* Avoid an infinite loop */
	//						pointer++;
	//					}

	//					pointer = &pointer[Output_End];
	//					start_point = start_point + Output_End ;
	//					wcstart_point = wcstart_point + wcOutput_End ;
	//				}
	//				else
	//				{
	//					if (answer != NO_MATCH)
	//					{
	//						pcre_error(fname,answer);
	//						freeArrayOfString(Str,mn);
	//						freeArrayOfString(Str2,mn2);
	//						return 0;
	//					}
	//				}
	//			}
	//			while( (answer == PCRE_FINISHED_OK) && (*pointer != '\0') &&  (typ != STR_ONCE) );

	//			if (save) {FREE(save);save=NULL;}
	//		}
	//		else
	//		{
	//			freeArrayOfString(Str,mn);
	//			freeArrayOfString(Str2,mn2);
	//			ScierrorW(999, _W("%ls: No more memory.\n"),fname);
	//			return 0;
	//		}
	//	}
	//}

	//numRow   = 1; /* Output values[] */
	//outIndex = 0;
	//CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues_start,&outIndex);
	//for ( i = 0 ; i < nbValues_start ; i++ )
	//{
	//	stk(outIndex)[i] = (double)wcvalues_start[i] ;
	//}
	//LhsVar(1) = Rhs+1 ;

	//if (Lhs >= 2)
	//{
	//	numRow   = 1;
	//	outIndex = 0;
	//	CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues_end,&outIndex);
	//	for ( i = 0 ; i < nbposition ; i++ )
	//	{
	//		stk(outIndex)[i] = (double)wcvalues_end[i] ;
	//	}
	//	LhsVar(2) = Rhs+2;
	//}

	//if (Lhs == 3)
	//{
	//	if (nbValues_start != 0)
	//	{
	//		char **match = (char**)MALLOC(sizeof(char*)*(nbValues_start));

	//		if (match == NULL)
	//		{
	//			freeArrayOfString(Str, mn);
	//			freeArrayOfString(Str2, mn2);

	//			ScierrorW(999, _W("%ls: No more memory.\n"),fname);
	//			return 0;
	//		}

	//		for( i = 0; i < nbValues_start; i++)
	//		{
	//			int len = values_end[i] - values_start[i] + 1;
	//			match[i] = (char*)MALLOC(sizeof(char)*(len + 1));
	//			strncpy(match[i], Str[0] + values_start[i] - 1, len);

	//			/* A char* always finished by \0 */
	//			match[i][len] = '\0';
	//		}

	//		numRow =  nbValues_start;
	//		outIndex = 1 ;
	//		CreateVarFromPtr(Rhs + 3,MATRIX_OF_STRING_DATATYPE, &numRow, &outIndex, match );
	//		LhsVar(3) = Rhs + 3 ;
	//		freeArrayOfString(match,nbValues_start);
	//	}
	//	else
	//	{
	//		int m3 = 0, n3 = 0, l3 = 0;
	//		CreateVar(Rhs+3,STRING_DATATYPE,  &m3, &n3, &l3);
	//		LhsVar(3) = Rhs + 3 ;
	//	}
	//}

	//freeArrayOfString(Str, mn);
	//freeArrayOfString(Str2, mn2);
	//if (values_start) {FREE(values_start); values_start = NULL;}
	//if (values_end) {FREE(values_end); values_end = NULL;}
	//if (wcvalues_start) {FREE(wcvalues_start); wcvalues_start = NULL;}
	//if (wcvalues_end) {FREE(wcvalues_end); wcvalues_end = NULL;}

	//C2F(putlhsvar)();
	//return 0;
}
/*-----------------------------------------------------------------------------------*/
