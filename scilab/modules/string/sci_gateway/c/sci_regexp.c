/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Cong WU
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
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
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "pcre.h"
#include "pcre_private.h"
#include "pcre_error.h"
#include "Scierror.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*------------------------------------------------------------------------*/
#define CHAR_S 's'
#define CHAR_R 'r'
#define STR_ONCE 'o'
/*------------------------------------------------------------------------*/
int C2F(sci_regexp)(char *fname,unsigned long fname_len)
{
    char typ = CHAR_S;
     char **Str = NULL;
    char **Str2 = NULL;
    int i = 0; /* loop indice */

    int mn = 0; /* dimension parameter 1 m*n */

    int m2 = 0,n2 = 0;
    int mn2 = 0; /* m2*n2 */

    int outIndex = 0;
    int numRow = 1;

    int *values_start = NULL;
    int *values_end= NULL;

    int nbValues_start = 0;
    int nbValues_end=0;
    int nbposition = 0;

    char **match = NULL;

    CheckRhs(1,3);
    CheckLhs(1,3);

    if (VarType(1) == sci_matrix)
    {
        int m1 = 0;
        int n1 = 0;

        GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&Str);
        if ((m1 == 0) && (n1 == 0))
        {
            int l = 0;
            CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l);
            LhsVar(1) = Rhs+1 ;
            C2F(putlhsvar)();
            return 0;
        }
    }
    else
    {
        int m1 = 0;
        int n1 = 0;

        GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);
        mn = m1*n1;
    }


    if (mn != 1)
    {
        freeArrayOfString(Str,mn);
        Scierror(36, _("%s: Wrong size for input argument #%d: Single string expected.\n"), fname,1);
        return 0;
    }

    GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Str2);
    mn2 = m2*n2;

    if ( (int)strlen(Str[0]) == 0 )
    {
        values_start = (int *)MALLOC(sizeof(int));
        values_end = (int *)MALLOC(sizeof(int));
    }
    else
    {
        values_start = (int *)MALLOC( sizeof(int) * ( strlen(Str[0]) ) );
        values_end = (int *)MALLOC( sizeof(int) * ( strlen(Str[0]) ) );
    }
    if (Rhs == 2 )
    {
        int x = 0;
        pcre_error_code answer = PCRE_FINISHED_OK;
        int start_point = 0;
        int Output_Start = 0;
        int Output_End = 0;

        for (x = 0; x < mn2; ++x)
        {
            char *save = strdup(Str2[x]);
            if (save)
            {
                char *pointer = Str[0];
                start_point = 0;
                do
                {
                    strcpy(save,Str2[x]);
					Output_Start = 0;
					Output_End = 0;

                    answer = pcre_private(pointer,save,&Output_Start,&Output_End);
                    if ( answer == PCRE_FINISHED_OK )
                    {
                        /* Start = End means that we matched a position and 0 characters.
                         * Matching 0 characters, for us, means no match.
                         */
                        if (Output_Start != Output_End)
                        {
                            /*adding the answer into the outputmatrix*/
                            values_start[nbValues_start++] = Output_Start + start_point + 1;
                            values_end[nbValues_end++] = Output_End + start_point;
                            
                            /*The number according to the str2 matrix*/
                            nbposition++;
                        }
                        else if(Output_End == 0 && *pointer != '\0')
                        {
                            /* Avoid an infinite loop */
                            pointer++;
                        }
                        
                        pointer = &pointer[Output_End];
                        start_point = start_point + Output_End ;
                    }
                    else
                    {
                        if (answer != NO_MATCH)
                        {
                            pcre_error(fname,answer);
                            return 0;
                        }
                    }
                }
                while( answer == PCRE_FINISHED_OK && *pointer != '\0' );

                if (save) {FREE(save);save=NULL;}
            }
            else
            {
                Scierror(999, _("%s: No more memory.\n"),fname);
                return 0;
            }
        }
    }

    if (Rhs == 3)
    {
        int m3 = 0;
        int n3 = 0;
        int l3 = 0;

        GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);

        if ( m3*n3 != 0) typ = cstk(l3)[0];

        if (typ== STR_ONCE)
        {
            int x = 0;
            pcre_error_code answer = PCRE_FINISHED_OK;

            int Output_Start = 0;
            int Output_End = 0;

            for (x = 0; x < mn2; ++x)
            {
                answer = pcre_private(Str[0], Str2[x], &Output_Start, &Output_End);
                if ( answer == PCRE_FINISHED_OK)
                {
                    /* Start = End means that we matched a position and 0 characters.
                     * Matching 0 characters, for us, means no match.
                     */
                    if (Output_Start!=Output_End)
                    {
                        /*adding the answer into the outputmatrix*/
                        values_start[nbValues_start++] = Output_Start+1;
                        values_end[nbValues_end++] = Output_End;
                        
                        /*The number according to the str2 matrix*/
                        nbposition++;
                    }
                }
                else
                {
                    if (answer != NO_MATCH)
                    {
                        pcre_error(fname,answer);
                        return 0;
                    }
                }
            }
        }
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: '%s' expected.\n"),fname,3,"o");
			return 0;
		}
    }

    if (nbValues_start!=0)
    {
        match = (char**)MALLOC(sizeof(char*)*(nbValues_start));
    }
    else
    {
        match = (char**)MALLOC(sizeof(char*)*(1));

    }

    for( i = 0; i < nbValues_start; i++)
    {
        int len = values_end[i] - values_start[i] + 1;
        match[i] = (char*)MALLOC(sizeof(char)*(len + 1));
        strncpy(match[i], Str[0] + values_start[i] - 1, len);
        
        /* A char* always finished by \0 */
        match[i][len] = '\0';
    }

    freeArrayOfString(Str, mn);
    freeArrayOfString(Str2, mn2);

    numRow   = 1;/* Output values[] */
    outIndex = 0;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues_start,&outIndex);
    for ( i = 0 ; i < nbValues_start ; i++ )
    {
        stk(outIndex)[i] = (double)values_start[i] ;
    }
    LhsVar(1) = Rhs+1 ;

    if (Lhs >= 2)
    {
        numRow   = 1;
        outIndex = 0;
        CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues_end,&outIndex);
        for ( i = 0 ; i < nbposition ; i++ )
        {
            stk(outIndex)[i] = (double)values_end[i] ;
        }
        LhsVar(2) = Rhs+2;
    }

    if (Lhs == 3)
    {
        numRow =  nbValues_start;
        outIndex = 1 ;
        CreateVarFromPtr(Rhs + 3,MATRIX_OF_STRING_DATATYPE, &numRow, &outIndex, match );
        LhsVar(3) = Rhs + 3 ;
    }

    C2F(putlhsvar)();

    if (values_start) {FREE(values_start); values_start = NULL;}
    if (values_end) {FREE(values_end); values_end = NULL;}

    freeArrayOfString(match,nbValues_start);
    return 0;
}
/*-----------------------------------------------------------------------------------*/
