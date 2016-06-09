/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2010 - DIGITEO - Antoine ELIAS
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

/* desc : search position of a character string in an other string
using regular express .                                         */
/*------------------------------------------------------------------------*/

#include "string_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"

extern "C"
{
#include "os_string.h"
#include "Scierror.h"
#include "localization.h"
#include "pcre.h"
#include "pcreposix.h"
#include "sci_malloc.h" /* MALLOC */
#include "charEncoding.h"
#include "pcre_private.h"
#include "pcre_error.h"
}

/*------------------------------------------------------------------------*/
#define GREP_OK             0
#define GREP_ERROR          1
#define MEMORY_ALLOC_ERROR -1
/*------------------------------------------------------------------------*/
typedef struct grep_results
{
    int sizeArraysMax;
    int currentLength;
    int *values;
    int *positions;
} GREPRESULTS;
/*------------------------------------------------------------------------*/
static int GREP_NEW(GREPRESULTS *results, char **Inputs_param_one, int mn_one, char **Inputs_param_two, int mn_two);
static int GREP_OLD(GREPRESULTS *results, char **Inputs_param_one, int mn_one, char **Inputs_param_two, int mn_two);
/*------------------------------------------------------------------------*/
types::Function::ReturnValue sci_grep(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bRegularExpression = false;

    //check input paramters
    if (in.size() < 2 || in.size() > 3)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "grep", 2, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(999, _("%s: Wrong number of output arguments: %d or %d expected.\n"), "grep", 1, 2);
        return types::Function::Error;
    }

    if (in[0]->isDouble() && in[0]->getAs<types::Double>()->getSize() == 0)
    {
        types::Double *pD = types::Double::Empty();
        out.push_back(pD);
        return types::Function::OK;
    }

    if (in.size() == 3)
    {
        //"r" for regular expression
        if (in[2]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "grep", 3);
            return types::Function::Error;
        }

        types::String* pS = in[2]->getAs<types::String>();
        if (pS->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "grep", 3);
            return types::Function::Error;
        }

        if (pS->get(0)[0] == 'r')
        {
            bRegularExpression = true;
        }
    }

    if (in[0]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "grep", 1);
        return types::Function::Error;
    }

    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "grep", 2);
        return types::Function::Error;
    }

    types::String* pS1 = in[0]->getAs<types::String>();
    types::String* pS2 = in[1]->getAs<types::String>();


    for (int i = 0 ; i < pS2->getSize() ; i++)
    {
        if (wcslen(pS2->get(i)) == 0)
        {
            Scierror(249, _("%s: Wrong values for input argument #%d: Non-empty strings expected.\n"), "grep", 2);
            return types::Function::Error;
        }
    }

    GREPRESULTS grepresults;
    int code_error_grep = GREP_OK;

    grepresults.currentLength = 0;
    grepresults.sizeArraysMax = 0;
    grepresults.positions = NULL;
    grepresults.values = NULL;

    char** pStr1 = (char**)MALLOC(sizeof(char*) * pS1->getSize());
    for (int i = 0 ; i < pS1->getSize() ; i++)
    {
        pStr1[i] = wide_string_to_UTF8(pS1->get(i));
    }

    char** pStr2 = (char**)MALLOC(sizeof(char*) * pS2->getSize());
    for (int i = 0 ; i < pS2->getSize() ; i++)
    {
        pStr2[i] = wide_string_to_UTF8(pS2->get(i));
    }

    if (bRegularExpression)
    {
        code_error_grep = GREP_NEW(&grepresults, pStr1, pS1->getSize(), pStr2, pS2->getSize());
    }
    else
    {
        code_error_grep = GREP_OLD(&grepresults, pStr1, pS1->getSize(), pStr2, pS2->getSize());
    }

    for (int i = 0; i < pS1->getSize(); i++)
    {
        FREE(pStr1[i]);
    }
    FREE(pStr1);

    for (int i = 0; i < pS2->getSize(); i++)
    {
        FREE(pStr2[i]);
    }
    FREE(pStr2);

    switch (code_error_grep)
    {
        case GREP_OK :
        {
            types::Double* pD1 = NULL;
            if (grepresults.currentLength == 0)
            {
                pD1 = types::Double::Empty();
            }
            else
            {
                pD1 = new types::Double(1, grepresults.currentLength);
                double* pDbl1 = pD1->getReal();
                for (int i = 0 ; i < grepresults.currentLength ; i++ )
                {
                    pDbl1[i] = static_cast<double>(grepresults.values[i]);
                }
            }

            out.push_back(pD1);

            if (_iRetCount == 2)
            {
                types::Double* pD2 = NULL;
                if (grepresults.currentLength == 0)
                {
                    pD2 = types::Double::Empty();
                }
                else
                {
                    pD2 = new types::Double(1, grepresults.currentLength);
                    double* pDbl2 = pD2->getReal();
                    for (int i = 0 ; i < grepresults.currentLength ; i++ )
                    {
                        pDbl2[i] = static_cast<double>(grepresults.positions[i]);
                    }
                }

                out.push_back(pD2);
            }

            if (grepresults.values)
            {
                FREE(grepresults.values);
                grepresults.values = NULL;
            }
            if (grepresults.positions)
            {
                FREE(grepresults.positions);
                grepresults.positions = NULL;
            }
        }
        break;

        case MEMORY_ALLOC_ERROR :
            Scierror(999, _("%s: No more memory.\n"), "grep");
        //no break, to free reserved memory.
        case GREP_ERROR :
        {
            if (grepresults.values)
            {
                FREE(grepresults.values);
                grepresults.values = NULL;
            }
            if (grepresults.positions)
            {
                FREE(grepresults.positions);
                grepresults.positions = NULL;
            }
            return types::Function::Error;
        }
        break;
    }

    return types::Function::OK;
}
//Function::ReturnValue sci_grep(typed_list &in, int _iRetCount, typed_list &out)
//{
//	CheckRhs(2,3);
//	CheckLhs(1,2);
//
//	if (VarType(1) == sci_matrix)
//	{
//		int m1 = 0, n1 = 0;
//		char **Str=NULL;
//
//		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&Str);
//
//		if ((m1 == 0) && (n1 == 0))
//		{
//			int l = 0;
//			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l);
//			LhsVar(1) = Rhs+1 ;
//			C2F(putlhsvar)();
//
//			return 0;
//		}
//	}
//
//	if (Rhs == 3)
//	{
//		if (VarType(3) == sci_strings)
//		{
//			char typ = 'd'; /*default */
//			int m3 = 0,n3 = 0,l3 = 0;
//
//			GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
//			if ( m3*n3 != 0) typ = cstk(l3)[0];
//
//			if (typ == 'r' )
//			{
//				sci_grep_common(fname,TRUE);
//			}
//			else
//			{
//				Scierror(999,_("%s: Wrong value for input argument #%d: ''%s'' expected.\n"),fname,3,"s");
//				return 0;
//			}
//		}
//		else
//		{
//			Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,3);
//			return 0;
//		}
//	}
//	else /* Rhs == 2 */
//	{
//		sci_grep_common(fname,FALSE);
//	}
//	return 0;
//}
/*-----------------------------------------------------------------------------------*/
static int GREP_NEW(GREPRESULTS *results, char **Inputs_param_one, int mn_one, char **Inputs_param_two, int mn_two)
{
    int x = 0, y = 0;
    char *save = NULL;
    int iRet = GREP_OK;
    pcre_error_code answer = PCRE_FINISHED_OK;
    results->sizeArraysMax = mn_one * mn_two;

    results->values = (int *)MALLOC(sizeof(int) * results->sizeArraysMax);
    results->positions = (int *)MALLOC(sizeof(int) * results->sizeArraysMax);

    if ( (results->values == NULL) || (results->positions == NULL) )
    {
        if (results->values)
        {
            FREE(results->values);
            results->values = NULL;
        }
        if (results->positions)
        {
            FREE(results->positions);
            results->positions = NULL;
        }
        return MEMORY_ALLOC_ERROR;
    }

    results->currentLength = 0;
    for ( y = 0; y < mn_one; ++y)
    {
        for ( x = 0; x < mn_two; ++x)
        {
            int Output_Start = 0;
            int Output_End = 0;
            save = os_strdup(Inputs_param_two[x]);
            answer = pcre_private(Inputs_param_one[y], save, &Output_Start, &Output_End, NULL, NULL);

            if (save)
            {
                FREE(save);
                save = NULL;
            }

            if ( answer == PCRE_FINISHED_OK )
            {
                results->values[results->currentLength] = y + 1;
                results->positions[results->currentLength] = x + 1;
                results->currentLength++;
            }
            else if (answer != NO_MATCH)
            {
                pcre_error("grep", answer);
                return GREP_ERROR;
            }
        }
    }

    return iRet;
}
/*-----------------------------------------------------------------------------------*/
static int GREP_OLD(GREPRESULTS *results, char **Inputs_param_one, int mn_one, char **Inputs_param_two, int mn_two)
{
    int x = 0, y = 0;

    results->values = (int *)MALLOC(sizeof(int) * (mn_one * mn_two + 1));
    results->positions = (int *)MALLOC(sizeof(int) * (mn_one * mn_two + 1));

    for (y = 0; y < mn_one; ++y)
    {
        for (x = 0; x < mn_two; ++x)
        {
            wchar_t* wcInputOne = to_wide_string(Inputs_param_one[y]);
            wchar_t* wcInputTwo = to_wide_string(Inputs_param_two[x]);

            if (wcInputOne && wcInputTwo)
            {
                if (wcsstr(wcInputOne, wcInputTwo) != NULL)
                {
                    results->values[results->currentLength] = y + 1;
                    results->positions[results->currentLength] = x + 1;
                    results->currentLength++;
                }
            }

            if (wcInputOne)
            {
                FREE(wcInputOne);
                wcInputOne = NULL;
            }
            if (wcInputTwo)
            {
                FREE(wcInputTwo);
                wcInputTwo = NULL;
            }
        }
    }
    return GREP_OK;
}
/*-----------------------------------------------------------------------------------*/
//static int sci_grep_common(char *fname,BOOL new_grep)
//{
//	int i = 0;
//
//	int m1 = 0, n1 = 0;
//	char **Strings_Input_One = NULL;
//	int m1n1 = 0; /* m1 * n1 */
//
//	int m2 = 0, n2 = 0;
//	char **Strings_Input_Two = NULL;
//	int m2n2 = 0; /* m2 * n2 */
//
//	GREPRESULTS grepresults;
//	int code_error_grep = GREP_OK;
//
//	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Strings_Input_One);
//	m1n1 = m1*n1;
//	GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Strings_Input_Two);
//	m2n2 = m2*n2;
//
//	for (i = 0;i < m2n2;i++)
//	{
//		if ( strlen(Strings_Input_Two[i]) == 0)
//		{
//			freeArrayOfString(Strings_Input_One,m1n1);
//			freeArrayOfString(Strings_Input_Two,m2n2);
//			Scierror(249,_("%s: Wrong values for input argument #%d: Non-empty strings expected.\n"),fname,2);
//			return 0;
//		}
//	}
//
//	grepresults.currentLength = 0;
//	grepresults.sizeArraysMax = 0;
//	grepresults.positions = NULL;
//	grepresults.values = NULL;
//
//	if (new_grep)
//	{
//		code_error_grep = GREP_NEW(&grepresults,Strings_Input_One,m1n1,Strings_Input_Two,m2n2);
//	}
//	else
//	{
//		code_error_grep = GREP_OLD(&grepresults,Strings_Input_One,m1n1,Strings_Input_Two,m2n2);
//	}
//
//	freeArrayOfString(Strings_Input_One,m1n1);
//	freeArrayOfString(Strings_Input_Two,m2n2);
//
//	switch (code_error_grep)
//	{
//	case GREP_OK :
//		{
//			int x = 0;
//			int numRow   = 0;
//			int outIndex = 0;
//
//			numRow   = 1;  /* Output values[]*/
//			outIndex = 0;
//			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&grepresults.currentLength,&outIndex);
//			for ( x = 0 ; x < grepresults.currentLength ; x++ )
//			{
//				stk(outIndex)[x] = (double)grepresults.values[x] ;
//			}
//			LhsVar(1) = Rhs+1 ;
//			if (Lhs == 2)
//			{
//				/* Output positions[]*/
//				numRow   = 1;
//				outIndex = 0;
//				CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&grepresults.currentLength,&outIndex);
//				for ( x = 0 ; x < grepresults.currentLength ; x++ )
//				{
//					stk(outIndex)[x] = (double)grepresults.positions[x] ;
//				}
//				LhsVar(2) = Rhs+2;
//			}
//			C2F(putlhsvar)();
//			if (grepresults.values) {FREE(grepresults.values); grepresults.values = NULL;}
//			if (grepresults.positions) {FREE(grepresults.positions); grepresults.positions = NULL;}
//		}
//		break;
//
//	case MEMORY_ALLOC_ERROR :
//		{
//			if (grepresults.values) {FREE(grepresults.values); grepresults.values = NULL;}
//			if (grepresults.positions) {FREE(grepresults.positions); grepresults.positions = NULL;}
//			Scierror(999,_("%s: No more memory.\n"),fname);
//		}
//		break;
//	}
//	return 0;
//}

/*-----------------------------------------------------------------------------------*/
