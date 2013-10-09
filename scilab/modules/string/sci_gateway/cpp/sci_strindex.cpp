/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Cong WU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : search position of a character string in an other string        */
/*        using regular expression .                                      */
/*------------------------------------------------------------------------*/
#include "funcmanager.hxx"
#include "string_gw.hxx"
#include "function.hxx"
#include "string.hxx"
#include "double.hxx"

extern "C"
{
#include <string.h>
#include <stdio.h>
#include "pcre.h"
#include "localization.h"
#include "pcre_private.h"
#include "pcre_error.h"
#include "Scierror.h"
}
/*------------------------------------------------------------------------*/
#define WCHAR_S L's'
#define WCHAR_R L'r'
/*------------------------------------------------------------------------*/
struct In
{
    int data;
    int position;
};
/*------------------------------------------------------------------------*/
int ComparaisonCallback( const void *in1 , const void *in2)
{
    return ((In*)in1)->data > ((In*)in2)->data ? 1 : -1;
}
/*------------------------------------------------------------------------*/

using namespace types;

types::Function::ReturnValue sci_strindex(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    bool bRegExp = false;
    if (in.size() < 2 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "strindex", 2, 3);
        return Function::Error;
    }

    if (in.size() > 2)
    {
        if (in[2]->isString() == false && in[2]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "strindex", 3);
            return Function::Error;
        }

        if (in[2]->getAs<types::String>()->get(0)[0] == WCHAR_R)
        {
            bRegExp = true;
        }
        else if (in[2]->getAs<types::String>()->get(0)[0] == WCHAR_S)
        {
            bRegExp = false;
        }
        else
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 's' or 'r' expected.\n"), "strindex", 3);
            return Function::Error;
        }
    }

    if (in[1]->isString() == false || (in[1]->getAs<types::String>()->getRows() != 1 && in[1]->getAs<types::String>()->getCols() != 1))
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string or a string vector expected.\n"), "strindex", 2);
        return Function::Error;
    }

    String* pS = in[1]->getAs<types::String>();
    wchar_t** pwstSearch = pS->get();

    if (in[0]->isDouble() && in[0]->getAs<Double>()->isEmpty())
    {
        out.push_back(Double::Empty());
        return Function::OK;
    }

    if (in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "strindex", 1);
        return Function::Error;
    }

    wchar_t* pwstData = in[0]->getAs<types::String>()->get()[0];
    if (wcslen(pwstData) == 0)
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount == 2)
        {
            out.push_back(types::Double::Empty());
        }

        return types::Function::OK;
    }

    In* pstrResult = new In[wcslen(pwstData)];

    //number of occurances
    int iValues = 0;
    if (bRegExp)
    {
        //pcre
        pcre_error_code iPcreStatus = PCRE_FINISHED_OK;
        for (int i = 0 ; i < pS->getSize() ; i++)
        {
            int iStart      = 0;
            int iEnd        = 0;
            int iStep       = 0;

            do
            {
                iPcreStatus = wide_pcre_private(pwstData + iStep, pwstSearch[i], &iStart, &iEnd, NULL, NULL);
                if (iPcreStatus == PCRE_FINISHED_OK)
                {
                    pstrResult[iValues].data        = iStart + iStep + 1;
                    pstrResult[iValues].position    = i + 1;
                    iStep                           += iEnd;
                    iValues++;
                }
                else
                {
                    if (iPcreStatus != NO_MATCH)
                    {
                        pcre_error("strindex", iPcreStatus);
                        delete[] pstrResult;
                        return Function::Error;
                    }
                    break;
                }
            }
            while (iPcreStatus == PCRE_FINISHED_OK && iStart != iEnd);
        }
    }
    else
    {
        for (int i = 0 ; i < pS->getSize() ; i++)
        {
            wchar_t* pCur = pwstData;
            do
            {
                pCur = wcsstr(pCur, pwstSearch[i]);
                if (pCur != NULL)
                {
                    pstrResult[iValues].data      = (int)(pCur - pwstData + 1);
                    pstrResult[iValues].position  = i + 1;
                    pCur++;
                    iValues++;
                }
            }
            while (pCur != NULL && pCur[0] != L'\0');
        }
    }

    qsort(pstrResult, iValues, sizeof(In), ComparaisonCallback);

    Double* pIndex = NULL;
    if (iValues == 0)
    {
        pIndex = Double::Empty();
    }
    else
    {
        pIndex = new Double(1, iValues);
        for (int i = 0 ; i < iValues ; i++)
        {
            pIndex->set(0, i, pstrResult[i].data);
        }
    }
    out.push_back(pIndex);

    if (_iRetCount == 2)
    {
        Double* pPos = NULL;
        if (iValues == 0)
        {
            pPos = Double::Empty();
        }
        else
        {
            pPos = new Double(1, iValues);
            for (int i = 0 ; i < iValues ; i++)
            {
                pPos->set(0, i, pstrResult[i].position);
            }
        }
        out.push_back(pPos);
    }

    delete[] pstrResult;
    return Function::OK;
    //BOOL bStrindex_with_pattern = FALSE;
    //int outIndex = 0;
    //int numRow = 1;
    //   int *next = NULL;
    //int i = 0;

    //CheckRhs(2,3);
    //CheckLhs(1,2);

    //if (Rhs == 3)
    //{
    //	int m3 = 0;
    //	int n3 = 0;
    //	char **Strings_Input3 = NULL;
    //	int m3n3 = 0; /* m3 * n3 */

    //	if (VarType(3) != sci_strings)
    //	{
    //		Scierror(999,_("%s: Wrong type for input argument #%d: Character expected.\n"),fname,3);
    //		return 0;
    //	}
    //	GetRhsVar(3, MATRIX_OF_STRING_DATATYPE, &m3, &n3, &Strings_Input3);
    //	m3n3 = m3 * n3;

    //	if (m3n3 != 1)
    //	{
    //		freeArrayOfString(Strings_Input3, m3n3);
    //		Scierror(999,_("%s: Wrong type for input argument #%d: Character expected.\n"), fname, 3);
    //		return 0;
    //	}

    //	if ( (strcmp(Strings_Input3[0],CHAR_R) == 0) || (strcmp(Strings_Input3[0],CHAR_S) == 0) )
    //	{
    //		if (strcmp(Strings_Input3[0],CHAR_R) == 0)
    //		{
    //			bStrindex_with_pattern = TRUE;
    //		}
    //		else
    //		{
    //			bStrindex_with_pattern = FALSE;
    //		}
    //		freeArrayOfString(Strings_Input3, m3n3);
    //	}
    //	else
    //	{
    //		freeArrayOfString(Strings_Input3, m3n3);
    //		Scierror(999,_("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"),fname,3,CHAR_S,CHAR_R);
    //		return 0;
    //	}
    //}

    //if (VarType(1) == sci_matrix)
    //{
    //	int m1 = 0;
    //	int n1 = 0;
    //	int l1 = 0;

    //	GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
    //	if ((m1 == 0) && (n1 == 0))
    //	{
    //		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
    //		LhsVar(1) = Rhs+1 ;
    //		C2F(putlhsvar)();
    //		return 0;
    //	}
    //	else
    //	{
    //		Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"),fname,3);
    //		return 0;
    //	}
    //}

    //if ( (VarType(1) == sci_strings) && (VarType(2) == sci_strings) )
    //{
    //	int m1 = 0, n1 = 0;
    //	char **Strings_Input1 = NULL;
    //	wchar_t *wStrings_Input1 = NULL;
    //	int m1n1 = 0; /* m1 * n1 */

    //	int m2 = 0, n2 = 0;
    //	char **Strings_Input2 = NULL;
    //	wchar_t **wStrings_Input2 = NULL;
    //	int m2n2 = 0; /* m2 * n2 */

    //	struct In *values=NULL;

    //	int nbValues = 0;
    //	int nbposition = 0;

    //	int i = 0;

    //	GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Strings_Input1);
    //	m1n1 = m1*n1;

    //	if (m1n1 != 1)
    //	{
    //		freeArrayOfString(Strings_Input1,m1n1);
    //		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
    //		return 0;
    //	}

    //	wStrings_Input1 = to_wide_string(Strings_Input1[0]);
    //	if (wStrings_Input1 == NULL)
    //	{
    //		/* string conversion fails */
    //		freeArrayOfString(Strings_Input1,m1n1);
    //		freeArrayOfString(Strings_Input2,m2n2);
    //		Scierror(999,_("%s: Wrong value for input argument #%d: A valid string expected (UTF-8 Encoding problem).\n"),fname,1);
    //		return 0;
    //	}

    //	GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Strings_Input2);
    //	m2n2 = m2*n2;

    //	if ( (m2 != 1) && (n2 != 1) )
    //	{
    //		freeArrayOfString(Strings_Input1,m1n1);
    //		freeArrayOfString(Strings_Input2,m2n2);
    //		Scierror(999,_("%s: Wrong type for input argument #%d: Row vector of strings or column vector of strings expected.\n"),fname,2);
    //		return 0;
    //	}

    //	wStrings_Input2 = (wchar_t**)MALLOC(m2n2 * sizeof(wchar_t*));
    //	for(i = 0 ; i < m2n2 ; i++)
    //	{
    //		wStrings_Input2[i] = to_wide_string(Strings_Input2[i]);
    //	}

    //	if ( (int)wcslen(wStrings_Input1) == 0 )
    //	{
    //		values= (struct In*)MALLOC(sizeof(struct In));
    //	}
    //	else
    //	{
    //		values = (struct In *)MALLOC( sizeof(struct In) * ( wcslen(wStrings_Input1) ) * m2n2);
    //	}

    //	if (bStrindex_with_pattern)
    //	{
    //		int x = 0;
    //		pcre_error_code w = PCRE_FINISHED_OK;

    //		int Output_Start = 0;
    //		int Output_End = 0;

    //		/* We use pcre library */
    //		for (x = 0; x < m2n2; ++x)
    //		{
    //			w = pcre_private(Strings_Input1[0],Strings_Input2[x],&Output_Start,&Output_End);
    //			if ( w == PCRE_FINISHED_OK)
    //			{
    //				char *partStr = os_strdup(Strings_Input1[0]);
    //				wchar_t *wcpartStr = NULL;
    //				partStr[Output_Start] = '\0';
    //				wcpartStr = to_wide_string(partStr);
    //				values[nbValues++].data = (int)wcslen(wcpartStr) + 1; /* adding the answer into the outputmatrix */
    //				values[nbposition++].position = x+1;        /* The number according to the str2 matrix */

    //				if (partStr) {FREE(partStr); partStr = NULL;}
    //				if (wcpartStr) {FREE(wcpartStr); wcpartStr = NULL;}
    //			}
    //			else
    //			{
    //				if (w != NO_MATCH)
    //				{
    //					freeArrayOfString(Strings_Input1,m1n1);
    //					freeArrayOfString(Strings_Input2,m2n2);
    //					pcre_error(fname,w);
    //					return 0;
    //				}
    //				break;
    //			}
    //		}

    //		qsort(values,nbValues,sizeof(values[0]),cmp);

    //	}
    //	else
    //	{
    //		/* We don't use pcre library */
    //		int x = 0;
    //		int pos = 0;

    //		for (x=0; x < m2n2 ;++x)
    //		{
    //			int w = 0;
    //			if ( wcslen(wStrings_Input2[x]) == 0 )
    //			{
    //				freeArrayOfString(Strings_Input2,m2n2);
    //				freeArrayOfString(Strings_Input1,m1n1);
    //				if (next) {FREE(next); next = NULL;}
    //				if (values) {FREE(values); values = NULL;}
    //				Scierror(999, _("%s: Wrong size for input argument #%d: Non-empty string expected.\n"), fname,2);
    //				return 0;
    //			}
    //			if (Strings_Input2)
    //			{
    //				wchar_t *pCur = wStrings_Input1;
    //				do
    //				{
    //					pCur = wcsstr(pCur, wStrings_Input2[x]);
    //					if (pCur != NULL)
    //					{
    //						pCur++;
    //						values[nbValues++].data = (int)(pCur - wStrings_Input1);
    //						values[nbposition++].position = x+1;
    //					}
    //				}
    //				while(pCur != NULL && *pCur != 0);//Plus tard

    //				/* values are sorted */
    //				qsort(values,nbValues,sizeof(values[0]),cmp);
    //			}
    //		}
    //	}

    //	FREE(wStrings_Input1);
    //	freeArrayOfWideString(wStrings_Input2, m2n2);
    //	freeArrayOfString(Strings_Input1,m1n1);
    //	freeArrayOfString(Strings_Input2,m2n2);

    //	numRow   = 1;
    //	outIndex = 0;
    //	CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues,&outIndex);
    //	for ( i = 0 ; i < nbValues ; i++ )
    //	{
    //		stk(outIndex)[i] = (double)values[i].data ;
    //	}
    //	LhsVar(1) = Rhs+1 ;

    //	if (Lhs == 2)
    //	{
    //		numRow   = 1;
    //		outIndex = 0;
    //		CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbposition,&outIndex);
    //		for ( i = 0 ; i < nbposition ; i++ )
    //		{
    //			stk(outIndex)[i] = (double)values[i].position ;
    //		}
    //		LhsVar(2) = Rhs+2;
    //	}

    //	C2F(putlhsvar)();

    //	if (values) {FREE(values); values = NULL;}
    //}
    //else
    //{
    //	if(VarType(1) != sci_strings)
    //	{
    //		Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
    //	}
    //	else
    //	{
    //		Scierror(999,_("%s: Wrong type for input argument #%d: Row vector of strings or column vector of strings expected.\n"),fname,2);
    //	}
    //	return 0;
    //}
    //return 0;
}
/*------------------------------------------------------------------------*/
