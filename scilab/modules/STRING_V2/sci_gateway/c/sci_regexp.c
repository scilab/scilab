/*------------------------------------------------------------------------*/
/* File: sci_regexp.c                                                     */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : search position of a character string in an other string
          using regular expression .                                      */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "gw_string.h"
#include "machine.h"
#include "pcre.h"
#include "pcreposix.h"
#include "stack-c.h"
#include "machine.h"
#include "MALLOC.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "pcre_private.h"
/*------------------------------------------------------------------------*/
#define CHAR_S 's'
#define CHAR_R 'r'
#define STR_ONCE 'o'
/* TODO : Add tests !!! man , bench , search memory leak , and optimize ... */
/* Not finish !!! :( */
/*------------------------------------------------------------------------*/
int C2F(sci_regexp) _PARAMS((char *fname,unsigned long fname_len))
{
	char typ = CHAR_S;
     char **Str = NULL;
	char **Str2 = NULL;
	char *pointer=NULL;
	char *save=NULL;

	int i = 0; /* loop indice */

    int mn = 0; /* dimension parameter 1 m*n */

    int m2 = 0,n2 = 0;
	int mn2 = 0; /* m2*n2 */

    int outIndex = 0;
    int numRow = 1;
    int j;

	int *values = NULL;
	int *values_end= NULL;
    int *position = NULL;

	int nbValues = 0;
	int nbValues_end=0;
    int nbposition = 0;
	int start_point=0;
	char **match=NULL;

	/* Are you sure for Lhs and Rhs */
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
        Scierror(36, _("First input argument must be one string.\n")); 
        return 0;
    }

    GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&Str2);
    mn2 = m2*n2;  

	if ( (int)strlen(Str[0]) == 0 ) 
	{
		values = (int *)MALLOC(sizeof(int));
		values_end = (int *)MALLOC(sizeof(int));
		position = (int *)MALLOC(sizeof(int));
	}
	else
	{
		values = (int *)MALLOC( sizeof(int) * ( strlen(Str[0]) ) );
		values_end = (int *)MALLOC( sizeof(int) * ( strlen(Str[0]) ) );
		position = (int *)MALLOC( sizeof(int) * ( strlen(Str[0]) ) );
	}

    if (Rhs >= 3)
    {
		int m3 = 0;
		int n3 = 0;
		int l3 = 0;

        GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);

        if ( m3*n3 != 0) typ = cstk(l3)[0];

		/* TODO : What do you when it is not 'R' ? ? ? */
        if (typ == CHAR_R )
        {
			int x = 0;
			int answer = 0;

			int Output_Start = 0;
			int Output_End = 0;
            
			/*When we use the regexp;*/
            for (x = 0; x < mn2; ++x)
            {
				save = (char *)MALLOC( sizeof(char) * ( strlen(Str2[x]) ) );
                pointer=Str[0];
				start_point=0;
				do
				{
					strcpy(save,Str2[x]);
					answer = pcre_private(pointer,save,&Output_Start,&Output_End);
					if ( answer == 0)
					{         
						if (Output_Start!=Output_End || Output_Start==0) 
						{
							values[nbValues++]=Output_Start+start_point+1;         /*adding the answer into the outputmatrix*/
						}
						else 
						{
							values[nbValues++]=Output_Start+start_point; 
						}
						values_end[nbValues_end++]=Output_End+start_point; 
						position[nbposition++]=x+1;                /*The number according to the str2 matrix*/
	                    pointer=pointer+Output_End;	
						start_point=start_point+Output_End;
					}
					else
					{
						switch (answer)
						{
						case NO_MATCH:
							/*No match */

						break;
						case NOT_ENOUGH_MEMORY_FOR_VECTOR:
							Scierror(999,_("Failed to get enough memory for offsets vector\n"));
							return 0;
							break;
						case DELIMITER_NOT_ALPHANUMERIC:
							Scierror(999,_("Delimiter must not be alphameric\n"));
							return 0;
							break;
						case CAPTURING_SUBPATTERNS_ERROR:
							Scierror(999,_("Capturing subpatterns error\n"));
							return 0;
						break;
						case PARTIAL_MATCHING_NOT_SUPPORTED:
							Scierror(999,_("Partial matching not supported\n"));
							return 0;
						break;
						case CONTAINS_EXPLICIT_CR_OR_LF_MATCH:
							Scierror(999,_("Contains explicit CR or LF match\n"));
							return 0;
						break;
						case DUPLICATE_NAME_STATUS_CHANGES:
							Scierror(999,_("Duplicate name status changes\n"));
							return 0;
						break;
						case TOO_BIG_FOR_OFFSET_SIZE:
							Scierror(999,_("Returned count is too big for offset size\n"));
							return 0;
						break;
						case LIMIT_NOT_RELEVANT_FOR_DFA_MATCHING:
							Scierror(999,_("Match limit not relevant for DFA matching: ignored\n"));
							return 0;
						break;
						default :
							 return 0;
						break;	

						}
					}
				}while(answer == 0 && Output_Start!=Output_End);
				if (save) {FREE(save);save=NULL;}
            }
		}
		else if (typ== STR_ONCE)
		{
		
			int x = 0;
			int answer = 0;

			int Output_Start = 0;
			int Output_End = 0;

            for (x = 0; x < mn2; ++x)
            {
                answer = pcre_private(Str[0],Str2[x],&Output_Start,&Output_End);
                if ( answer == 0)
                {      
					if (Output_Start!=Output_End || Output_Start==0) 
					{
						values[nbValues++]=Output_Start+1;         /*adding the answer into the outputmatrix*/

					}
					else
					{
						values[nbValues++]=Output_Start;
					}
					
					values_end[nbValues_end++]=Output_End; 
                    position[nbposition++]=x+1;                /*The number according to the str2 matrix*/
                }    
				else
				{
					switch (answer)
					{
					case NO_MATCH:
						/*No match */

					break;
					case NOT_ENOUGH_MEMORY_FOR_VECTOR:
						Scierror(999,_("Failed to get enough memory for offsets vector\n"));
						return 0;
						break;
					case DELIMITER_NOT_ALPHANUMERIC:
						Scierror(999,_("Delimiter must not be alphameric\n"));
						return 0;
						break;
					case CAPTURING_SUBPATTERNS_ERROR:
						Scierror(999,_("Capturing subpatterns error\n"));
						return 0;
					break;
					case PARTIAL_MATCHING_NOT_SUPPORTED:
						Scierror(999,_("Partial matching not supported\n"));
						return 0;
					break;
					case CONTAINS_EXPLICIT_CR_OR_LF_MATCH:
						Scierror(999,_("Contains explicit CR or LF match\n"));
						return 0;
					break;
					case DUPLICATE_NAME_STATUS_CHANGES:
						Scierror(999,_("Duplicate name status changes\n"));
						return 0;
					break;
					case TOO_BIG_FOR_OFFSET_SIZE:
						Scierror(999,_("Returned count is too big for offset size\n"));
						return 0;
					break;
					case LIMIT_NOT_RELEVANT_FOR_DFA_MATCHING:
						Scierror(999,_("Match limit not relevant for DFA matching: ignored\n"));
						return 0;
					break;
					default :
						 return 0;
					break;	

					}
				}
            }
        
		}
	}
    
	if (nbValues!=0)
	{
		match = (char**)MALLOC(sizeof(char*)*(nbValues));
	}
	else
	{
		match = (char**)MALLOC(sizeof(char*)*(1));

	}
	for( i=0;i<nbValues;i++)
	{
		if (values_end[i]-values[i]>1) match[i] = (char*)MALLOC(sizeof(char)*(values_end[i]-values[i]+1));
		else match[i] = (char*)MALLOC(sizeof(char)*(1));
		for(j=values[i]-1;j<values_end[i];j++)
		{
			match[i][j+1-values[i]]=Str[0][j];
		}
	}


	freeArrayOfString(Str,mn);
	freeArrayOfString(Str2,mn2);



    numRow   = 1;/* Output values[] */ 
    outIndex = 0;
    CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&numRow,&nbValues,&outIndex);
    for ( i = 0 ; i < nbValues ; i++ )
    {
		stk(outIndex)[i] = (double)values[i] ;
    }
    LhsVar(1) = Rhs+1 ;
    
	if (Lhs>=2)
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

    if (Lhs==3)
	{
		numRow =  nbValues;
		outIndex = 1 ;
		CreateVarFromPtr(Rhs + 3,MATRIX_OF_STRING_DATATYPE, &numRow, &outIndex, match );
		LhsVar(3) = Rhs + 3 ;
	}

    C2F(putlhsvar)();

    if (values) {FREE(values); values = NULL;}
	if (values_end) {FREE(values_end); values_end = NULL;}
    if (position) {FREE(position); position = NULL;}
	
    freeArrayOfString(match,nbValues);
    return 0;
}
/*-----------------------------------------------------------------------------------*/
