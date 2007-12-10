/*------------------------------------------------------------------------*/
/* File: sci_strsubst1.c                                                  */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                      */
/* desc : substitute a character string by another in a character string
          using regular expression .                                         */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "machine.h"
#include "MALLOC.h"
#include "kmp.h"
#include "pcre.h"
#include "pcreposix.h"
#include "pcre_internal.h"
#include "pcre_private.h"
/*-------------------------------------------------------------------------------------*/
static int *next = NULL;
static char * newstr(const char*str, unsigned start, unsigned end, char*_replacedstr1);
/*-------------------------------------------------------------------------------------*/
int C2F(sci_strsubst) _PARAMS((char *fname,unsigned long fname_len))
{
    int numRow = 0;
    int numCol = 0;
    int lnumRow = 0;
    int lnumCol = 0;
    int loutIndex = 0;
    char *_replacedstr = NULL;
    char **Output_StringMatrix = NULL;
    char typ = NULL;
    char *Middle=NULL;
	char **Input_StringMatrix_One=NULL;
    char **Input_StringMatrix_Two=NULL;
	char **Input_StringMatrix_Three=NULL;
    int x = 0,Row_One = 0,Col_One = 0,Number_of_Matrix_One = 0,Number_of_Matrix_Two = 0,Row_Three = 0,Row_Two = 0,Col_Two = 0,Number_of_Matrix_Three = 0,Col_Three = 0,m4 = 0,n4 = 0,l4 = 0;
    int i = 0, l = 0;
    char *pattern = NULL;
	int answer;
    int Output_Start;
    int Output_End;

    CheckRhs(1,4);

    if (VarType(1) == sci_matrix)
	{
		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&Row_One,&Col_One,&Input_StringMatrix_One);
		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&Row_Two,&Col_Two,&Input_StringMatrix_Two);
        if ((Row_Two == 0) && (Col_Two == 0))
		{
			 Scierror(246,"function not defined for given argument type(s), check arguments or define function %s for overloading\r\n",fname);
		     return 0;
		}

		if ((Row_One == 0) && (Col_One == 0))
		{
            l = 0;
            CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&Row_One,&Col_One,&l);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
			return 0;

		}
	}

	if ( VarType(1) == sci_strings)
		{
			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_StringMatrix_One);
			Number_of_Matrix_One = Row_One*Col_One;  
			GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&Row_Two,&Col_Two,&Input_StringMatrix_Two);
			Number_of_Matrix_Two = Row_Two*Col_Two;  
			GetRhsVar(3,MATRIX_OF_STRING_DATATYPE,&Row_Three,&Col_Three,&Input_StringMatrix_Three);
			Number_of_Matrix_Three = Row_Three*Col_Three;
			pattern=*Input_StringMatrix_Two;
	
	
			if ( (int)strlen(Input_StringMatrix_One[0]) == 0) 
				{
					next=(int *)MALLOC(sizeof(int)*(1));
				}
			else 
				{
					next=(int *)MALLOC(sizeof(int)*(strlen(Input_StringMatrix_One[0])*strlen(Input_StringMatrix_One[0]))); 
				}
			Output_StringMatrix = (char**)MALLOC(sizeof(char*)*(Number_of_Matrix_One+1));

			for (i = 0; i < Number_of_Matrix_One ;i++)
				{
					if (strlen(Input_StringMatrix_One[i]) == 0) 
						{
							Output_StringMatrix[i]=(char*)MALLOC(sizeof(char)*(strlen("")+1));
							if (Output_StringMatrix[i]) strcpy(Output_StringMatrix[i],"");
						}
					else
						{
							Output_StringMatrix[i]=(char*)MALLOC(sizeof(char)*(strlen(Input_StringMatrix_One[i])+1));
							if (Output_StringMatrix[i]) strcpy(Output_StringMatrix[i],Input_StringMatrix_One[i]);
							else 
								{
									Scierror(999,"%s : Error memory allocation.\r\n",fname);
									return 0;
								}
			
						}
				}


			if (Rhs >= 4)
				{
					GetRhsVar(4,STRING_DATATYPE,&m4,&n4,&l4);
					if ( m4*n4 != 0)
						typ = cstk(l4)[0];
					if (typ == 'r' )      /*When we want to use the regular expression to do with this substring */
						{  
							for (x = 0; x < Number_of_Matrix_One;++x)
								{
									answer=pcre_private(Input_StringMatrix_One[x],Input_StringMatrix_Two[0],&Output_Start,&Output_End);
									if (answer==0)
										{
											_replacedstr = newstr(Output_StringMatrix[x], Output_Start, Output_End,*Input_StringMatrix_Three);
											if (Output_StringMatrix[x]) FREE(Output_StringMatrix[x]);
											Output_StringMatrix[x] = _replacedstr;
										}    
								}
						}
				}
			else
				{
					for (x=0;x<Number_of_Matrix_One;x++)
						{
							if (strlen(Output_StringMatrix[x]) == 0)  break;
	
							if (!Output_StringMatrix[x]) break;

							if (strlen(Input_StringMatrix_Two[0])==0)
								{
									Scierror(999, "2th argument must not be an empty string.\n");
									return 1;
								}
							Middle=(char*)MALLOC(sizeof(char)*(strlen(Input_StringMatrix_Two[0])+1));
							for (i=0; i<(int)strlen(Output_StringMatrix[x]);i++)
								{
									if (Output_StringMatrix[x]==NULL) 
										{
											Scierror(999,"%s : Error memory allocation.\r\n",fname);
											return 0;
										}
									//getnext(Input_StringMatrix_Two[0],next);
									strncpy(Middle,Output_StringMatrix[x]+i,strlen(Input_StringMatrix_Two[0]));

									if (Output_StringMatrix[x]) 
										{
					
										}
									else 
										{
											Scierror(999,"%s : Error memory allocation.\r\n",fname);
											return 0;
										}

									if (strcmp(Middle,Input_StringMatrix_Two[0])==0)
										{
											_replacedstr = newstr(Output_StringMatrix[x], i, i+1 + (int)strlen(Input_StringMatrix_Two[0])-1,*Input_StringMatrix_Three);
											if (Output_StringMatrix[x]) FREE(Output_StringMatrix[x]);
											Output_StringMatrix[x] = _replacedstr;
											if (strlen(Input_StringMatrix_Three[0])>strlen(Input_StringMatrix_Two[0])) 
												{
													i=i+((int)strlen(Input_StringMatrix_Three[0])-(int)strlen(Input_StringMatrix_Two[0]));
												}
										}
								}
							lnumRow   = 1 ;
							lnumCol   = Number_of_Matrix_One ;
							loutIndex = 0 ;
						}
				}
			numRow   = Row_One ;
			numCol   = Col_One ;
			CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE, &numRow, &numCol, Output_StringMatrix );
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
			if (next) {FREE(next); next=NULL;}
    
			for (i=0;i<Number_of_Matrix_One;i++)
				{
					if(Output_StringMatrix[i])  {FREE(Output_StringMatrix[i]); Output_StringMatrix[i]=NULL;}
				}
			if (strlen(Input_StringMatrix_One[0])!=0)  
				{
					if (Output_StringMatrix) {FREE(Output_StringMatrix); Output_StringMatrix=NULL;}
				}

			return 0;
		}
/* If it is a constant*/
   //n = 0;
   //m = 0;
   //l = 0;
   //CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&n,&m,&l);
   //LhsVar(1) = Rhs+1 ;
   //C2F(putlhsvar)();
   //return 0;
}


/*-------------------------------------------------------------------------------------*/
static char * newstr(const char*str, unsigned start, unsigned end, char*_replacedstr1)
{
	char *output = NULL;
	int len = 0;
	int n = end - start;
	char *stbuf1 = NULL;                
	char *stbuf2  = NULL;              
	char *stbuf3 = NULL;

	stbuf1=(char *)MALLOC(sizeof(char)*(start+1));
	stbuf2=(char *)MALLOC(sizeof(char)*(strlen(_replacedstr1)+1));
	stbuf3=(char *)MALLOC(sizeof(char)*(strlen(str)-n+strlen(_replacedstr1)+1));

	/* TODO: added some tests about MALLOC*/

	strncpy(stbuf1, str , start);
	stbuf1[start]='\0';

	strncpy(stbuf2, _replacedstr1,strlen(_replacedstr1));
	stbuf2[strlen(_replacedstr1)]='\0';

	strncpy(stbuf3, str+end ,strlen(str)-n+strlen(_replacedstr1) );
	stbuf3[strlen(str)-n+strlen(_replacedstr1)]='\0';


	len = start + strlen(_replacedstr1) + strlen(str)-n+strlen(_replacedstr1);
	output=(char*)MALLOC(sizeof(char)*(len+1));
	if (output)
	{
		strcpy(output,"");
		strcat(output,stbuf1);
		strcat(output,stbuf2);
		strcat(output,stbuf3);
	}


	if (stbuf1) {FREE(stbuf1); stbuf1=NULL;}
	if (stbuf2) {FREE(stbuf2); stbuf2=NULL;}
	if (stbuf3) {FREE(stbuf3); stbuf3=NULL;}

	return output;

}
/*-------------------------------------------------------------------------------------*/
