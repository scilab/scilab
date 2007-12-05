/*------------------------------------------------------------------------*/
/* File: sci_strcat.c                                                     */
/* Copyright INRIA 2007                                                   */ 
/* @Authors : Cong Wu, A.C                                                */
/* desc : catenate character strings                                      */
/* Examples: strcat(string(1:10),',')                                     */ 
/*                                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h" 
#include "stack-c.h"
#include "machine.h"
#include "MALLOC.h"
#include "Scierror.h"
/*-------------------------------------------------------------------------------------*/ 
#define STAR '*'
#define COL 'c'
#define ROW 'r'
#define ONE_CHAR 1
#define EMPTY_CHAR ""
/*-------------------------------------------------------------------------------------*/ 
static int sci_strcat_three_rhs(char *fname);
static int sci_strcat_two_rhs(char *fname);
static int sci_strcat_one_rhs(char *fname);
static int sci_strcat_rhs_one_is_a_matrix(char *fname);
/*-------------------------------------------------------------------------------------*/
int C2F(sci_strcat) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(1,3);
	CheckLhs(1,1);

	switch (Rhs)
	{
		case 3:
		{
			sci_strcat_three_rhs(fname);
		}
		break;
		case 2:
		{
			sci_strcat_two_rhs(fname);
		}
		break;
		case 1:
		{
			sci_strcat_one_rhs(fname);
		}
		break;
		default:
		/* nothing */
		break;
	}
	return 0;
}
/*-------------------------------------------------------------------------------------*/
/* this case is not documented in help :( */
/* TO DO : update help or remove this case */
static int sci_strcat_three_rhs(char *fname) 
{ 
	char typ = NULL;
	char **Input_String_One = NULL,**Output_String = NULL;
	static char def_sep[] ="";
	char *Input_String_Two = def_sep;
	static int un=1;
	int Row_One = 0,Col_One = 0;
	int mn = 0;
	int i = 0,j = 0,k = 0;
	int Row_Two = 0,Col_Two = 0;
	int Row_Three = 0,Col_Three = 0;
	int l2 = 0,l3 = 0,nchars = 0; 

	switch ( VarType(1)) 
	{
	case sci_strings :
		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_String_One);
		mn = Row_One*Col_One;  
		if (Rhs >= 2) 
		{ 
			GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&Row_Two,&Col_Two,&Input_String_Two);
			//Input_String_Two = cstk(l2);
		}
		if (Rhs >= 3) 
		{
			GetRhsVar(3,STRING_DATATYPE,&Row_Three,&Col_Three,&l3); 
			if ( Row_Three*Col_Three != 0) 
				typ = cstk(l3)[0];
			if (typ != COL && typ != ROW ) 
			{
				Scierror(999,"%s: third argument should be 'c' or 'r'\r\n",fname); 
				return 0;
			}
		}
		switch ( typ ) 
		{
		case STAR : 
			/* just return one string */ 
			for ( i = 0 ; i < mn ; i++ ) nchars += (int)strlen(Input_String_One[i]); 
			nchars += (mn-1)*(int)strlen(Input_String_Two);
			CreateVar(Rhs+1,STRING_DATATYPE,&un,&nchars,&l3);
			k=0;
			for ( i = 0 ; i < mn ; i++ ) 
			{
				for ( j =0 ; j < (int)strlen(Input_String_One[i]) ; j++ ) *cstk(l3+ k++) = Input_String_One[i][j]; 
				if ( i != mn-1) for ( j =0 ; j < (int)strlen(Input_String_Two) ; j++ ) *cstk(l3+ k++) = Input_String_Two[j];
			}
			if (Input_String_One)
			{
				for ( i = 0 ; i < mn ; i++ ) 
				{
					if (Input_String_One[i])
					{
						FREE(Input_String_One[i]);
						Input_String_One[i]=NULL;
					}
				} 
				FREE(Input_String_One);
				Input_String_One=NULL;
			}
			LhsVar(1) = Rhs+1  ;

			
			if (Input_String_Two)  freeArrayOfString(Input_String_One,Row_Two*Col_Two);
			
			break;
		case COL: 
			/* return a column matrix */ 
			if ( (Output_String = MALLOC((Row_One+1)*sizeof(char *)))==NULL) 
			{
				Scierror(999,"%s: Out of memory\r\n",fname);
				return 0;
			}
			Output_String[Row_One]=NULL;
			for (i= 0 ; i < Row_One ; i++) 
			{
				/* length of row i */ 
				nchars = 0;
				for ( j = 0 ; j < Col_One ; j++ ) nchars += (int)strlen(Input_String_One[i+ Row_One*j]);
				nchars += (Col_One-1)*(int)strlen(Input_String_Two); 
				if ( (Output_String[i]=MALLOC((nchars+1)*sizeof(char)))==NULL) 
				{
					Scierror(999,"%s: Out of memory\r\n",fname);
					return 0;
				} 
				/* fill the string */ 
				strcpy(Output_String[i],Input_String_One[i]); 
				for ( j = 1 ; j < Col_One ; j++ ) 
				{
					strcat(Output_String[i],Input_String_Two); 
					strcat(Output_String[i],Input_String_One[i+ Row_One*j]);
				}
			}
			CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &Row_One, &un, Output_String);
			if (Input_String_One) 
			{
				for ( i = 0 ; i < mn ; i++ )
				{
					if (Input_String_One[i])
					{
						FREE(Input_String_One[i]);
						Input_String_One[i]=NULL; 
					}
				}
				FREE(Input_String_One);
				Input_String_One=NULL;
			}

            if (Input_String_Two) 
			{
				for ( i = 0 ; i <Row_Two*Col_Two ; i++ )
				{
					if (Input_String_Two[i])
					{
						FREE(Input_String_Two[i]);
						Input_String_Two[i]=NULL; 
					}
				}
				FREE(Input_String_Two);
				Input_String_Two=NULL;
			}


			LhsVar(1) = Rhs+1  ;
			if (Output_String)
			{
				int i2=0;
				for (i2=0;i2<Row_One+1;i2++)
				{
					if (Output_String[i2])
					{
						FREE(Output_String[i2]);
						Output_String[i2]=NULL; 
					}
				}
				FREE(Output_String);
				Output_String=NULL;
			}
			break;
		case ROW: 
			/* return a row matrix */ 
			if ( (Output_String = MALLOC((Col_One+1)*sizeof(char *)))==NULL) 
			{
				Scierror(999,"%s: Out of memory\r\n",fname);
				return 0;
			}
			Output_String[Col_One]=NULL;
			for (j= 0 ; j < Col_One ; j++) 
			{
				/* length of col j */ 
				nchars = 0;
				for ( i = 0 ; i < Row_One ; i++ ) 
					nchars += (int)strlen(Input_String_One[i+ Row_One*j]);
				nchars += (Row_One-1)*(int)strlen(Input_String_Two); 
				if ( (Output_String[j]=MALLOC((nchars+1)*sizeof(char)))==NULL) 
				{
					Scierror(999,"%s: Out of memory\r\n",fname);
					return 0;
				} 
				/* fill the string */ 
				strcpy(Output_String[j],Input_String_One[j*Row_One]); 
				for ( i = 1 ; i < Row_One ; i++ ) {
					strcat(Output_String[j],Input_String_Two); 
					strcat(Output_String[j],Input_String_One[i+ Row_One*j]);
				}
			}
			CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &un, &Col_One, Output_String);
			if (Input_String_One) 
			{
				for ( i = 0 ; i < mn ; i++ )
				{
					if (Input_String_One[i])
					{
						FREE(Input_String_One[i]);
						Input_String_One[i]=NULL; 
					}
				}
				FREE(Input_String_One);
				Input_String_One=NULL;
			}


			if (Input_String_Two) 
			{
				for ( i = 0 ; i <Row_Two*Col_Two ; i++ )
				{
					if (Input_String_Two[i])
					{
						FREE(Input_String_Two[i]);
						Input_String_Two[i]=NULL; 
					}
				}
				FREE(Input_String_Two);
				Input_String_Two=NULL;
			}
			if (Output_String)
			{
				int i2=0;
				for (i2=0;i2<Col_One+1;i2++) 
				{
					if (Output_String[i2])
					{
						FREE(Output_String[i2]);
						Output_String[i2]=NULL;
					}
				} 
				FREE(Output_String);
				Output_String=NULL;
			}
			LhsVar(1) = Rhs+1  ;
			break;
		}
		break; 
	default : 
		OverLoad(1);
		break; 
	}

	C2F(putlhsvar)();
	return 0;
}
/*-------------------------------------------------------------------------------------*/
static int sci_strcat_two_rhs(char *fname)
{
	int Type_One = VarType(1);
	int Type_Two = VarType(2);

	int Number_Inputs_Two = 0;
	char **Input_String_Two = NULL;

	if (Type_Two != sci_strings)
	{
		Scierror(246,"function not defined for given argument type(s), check arguments or define function %s for overloading",fname); 
		return 0;
	}
	else /* sci_strings */
	{
		int Row_Two = 0,Col_Two = 0;

		GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&Row_Two,&Col_Two,&Input_String_Two);
		Number_Inputs_Two = Row_Two * Col_Two;
		/* check we have only a string as second parameter */
		if (Number_Inputs_Two != 1)
		{
			Scierror(36,"%s : 2th argument is incorrect here.\n",fname); 
			return 0;
		}
	}

	if ( (Type_One != sci_strings) && (Type_One != sci_matrix) )
	{
		Scierror(246,"function not defined for given argument type(s), check arguments or define function %s for overloading.\n",fname); 
		return 0;
	}
	else
	{
		if (Type_One == sci_matrix)
		{
			sci_strcat_rhs_one_is_a_matrix(fname);
		}
		else /* sci_strings */
		{
			char **Input_String_One = NULL;
			int Row_One = 0,Col_One = 0;
			int Number_Inputs_One = 0;
			int length_ouput = 0;

			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_String_One);
			Number_Inputs_One = Row_One * Col_One;

			if (Input_String_One)
			{
				if ( (Number_Inputs_One == 1) && (strcmp(Input_String_One[0],"")==0) )
				{
					/* With no input argument returns a zero length character string */
					int m1 = 0, n1 = 0, l1 = 0;

					CreateVar(Rhs+1,STRING_DATATYPE,  &m1, &n1, &l1);
					LhsVar(1)=Rhs+1;
					C2F(putlhsvar)();
					return 0;
				}
				else
				{
					int i = 0;
					for (i = 0; i < Number_Inputs_One; i++)
					{
						if (Input_String_One[i])
						{
							length_ouput = length_ouput + (int)strlen(Input_String_One[i])+(int)strlen(Input_String_Two[0]);
						}
					}
				}
			}

			if (length_ouput > 0)
			{
				static int n1 = 0, m1 = 0;
				int outIndex = 0 ;
				char *Output_String = NULL;
				Output_String = (char *)MALLOC( sizeof(char) * (length_ouput + 1) );

				if (Output_String)
				{
					int i = 0;
					for (i = 0; i < Number_Inputs_One; i++)
					{
						if ( i == 0)
						{
							strcpy(Output_String,Input_String_One[i]);
							if (i<Number_Inputs_One - 1) strcat(Output_String,Input_String_Two[0]);
						}
						else
						{
							strcat(Output_String,Input_String_One[i]);
							if (i<Number_Inputs_One - 1) strcat(Output_String,Input_String_Two[0]);
						}
					}

					m1= (int)strlen(Output_String);
					n1=1;

					CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
					strcpy(cstk(outIndex),Output_String );

					LhsVar(1) = Rhs+1;
					C2F(putlhsvar)();

					FREE(Output_String);
					Output_String = NULL;
					freeArrayOfString(Input_String_One,Row_One*Col_One);
					freeArrayOfString(Input_String_Two,Number_Inputs_Two);
				}
				else
				{
					Scierror(999,"Error memory allocation.\n");
				}
			}
			else
			{
				if (length_ouput == 0)
				{
					char **Output_StringMatrix = (char**)MALLOC(sizeof(char*)*(ONE_CHAR));
					if (Output_StringMatrix)
					{
						Output_StringMatrix[0] = (char*)MALLOC(sizeof(char)*(ONE_CHAR));
						if (Output_StringMatrix[0])
						{
							strcpy(Output_StringMatrix[0],EMPTY_CHAR);
							Row_One = 1;
							Col_One = 1;
							CreateVarFromPtr(Rhs + 1,MATRIX_OF_STRING_DATATYPE,&Row_One, &Col_One, Output_StringMatrix); 
							LhsVar(1) = Rhs + 1;
							C2F(putlhsvar)();
							FREE(Output_StringMatrix[0]); Output_StringMatrix[0] = NULL;
							FREE(Output_StringMatrix); 	Output_StringMatrix = NULL;
							return 0;
						}
						else
						{
							FREE(Output_StringMatrix); Output_StringMatrix = NULL;
							Scierror(999,"Error memory allocation.\n"); 
						}
					}
					else
					{
						Scierror(999,"Error memory allocation.\n"); 
					}		
				}
				else
				{
					Scierror(999,"%s : incorrect argument(s).\n",fname);
				}
			}

		}
	}
	return 0;
}
/*-------------------------------------------------------------------------------------*/
static int sci_strcat_one_rhs(char *fname)
{
	int Type_One = VarType(1);
	if ( (Type_One != sci_strings) && (Type_One != sci_matrix) )
	{
		Scierror(246,"function not defined for given argument type(s), check arguments or define function %s for overloading",fname); 
		return 0;
	}
	else
	{
		if (Type_One == sci_strings)
		{
			char **Input_String_One = NULL;
			int Row_One = 0,Col_One = 0;
			int Number_Inputs = 0;
			int length_ouput = 0;

			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_String_One);
			Number_Inputs = Row_One * Col_One;
			if (Input_String_One)
			{
				int i = 0;
				for (i = 0; i < Number_Inputs; i++)
				{
					if (Input_String_One[i])
					{
						length_ouput = length_ouput + (int)strlen(Input_String_One[i]);
					}
				}
			}

			if (length_ouput > 0)
			{
				static int n1 = 0, m1 = 0;
				int outIndex = 0 ;
				char *Output_String = NULL;
				Output_String = (char *)MALLOC( sizeof(char) * (length_ouput+1) );
				if (Output_String)
				{
					int i = 0;
					for (i = 0; i < Number_Inputs; i++)
					{
						if ( i == 0)
						{
							strcpy(Output_String,Input_String_One[i]);
						}
						else
						{
							strcat(Output_String,Input_String_One[i]);
						}
					}

					m1= (int)strlen(Output_String);
					n1=1;

					CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
					strcpy(cstk(outIndex),Output_String );

					LhsVar(1) = Rhs+1;
					C2F(putlhsvar)();

					FREE(Output_String);
					Output_String = NULL;
					if (Input_String_One)  freeArrayOfString(Input_String_One,Number_Inputs);

				}
				else
				{
					Scierror(999,"Error memory allocation.\n");
				}
			}
			else
			{
				Scierror(999,"%s : incorrect argument(s).\n",fname);
			}
		}
		else /* sci_matrix*/
		{
			sci_strcat_rhs_one_is_a_matrix(fname);
		}
	}
	return 0;
}
/*-------------------------------------------------------------------------------------*/
static int sci_strcat_rhs_one_is_a_matrix(char *fname)
{
	/* strcat([],'A') returns a empty string matrix */
	char **Input_String_One = NULL;
	char **Output_StringMatrix = NULL;
	int Row_One = 0,Col_One = 0;

	GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&Row_One,&Col_One,&Input_String_One);

	/* check that we have [] */
	if ((Row_One == 0) && (Col_One == 0)) 
	{
		Output_StringMatrix = (char**)MALLOC(sizeof(char*)*(ONE_CHAR));
		if (Output_StringMatrix)
		{
			Output_StringMatrix[0] = (char*)MALLOC(sizeof(char)*(ONE_CHAR));
			if (Output_StringMatrix[0])
			{
				strcpy(Output_StringMatrix[0],EMPTY_CHAR);
				Row_One = 1;
				Col_One = 1;
				CreateVarFromPtr(Rhs + 1,MATRIX_OF_STRING_DATATYPE,&Row_One, &Col_One, Output_StringMatrix); 
				LhsVar(1) = Rhs + 1;
				C2F(putlhsvar)();
				FREE(Output_StringMatrix[0]); Output_StringMatrix[0] = NULL;
				FREE(Output_StringMatrix); 	Output_StringMatrix = NULL;
				//freeArrayOfString(Input_String_One,Row_One*Col_One);
			}
			else
			{
				FREE(Output_StringMatrix); Output_StringMatrix = NULL;
				//freeArrayOfString(Input_String_One,Row_One*Col_One);
				Scierror(999,"Error memory allocation.\n"); 
			}
		}
		else
		{
			Scierror(999,"Error memory allocation.\n"); 
		}
	}
	else
	{
		Scierror(999,"%s : first argument has a wrong type, expecting scalar or string matrix.\n"); 
	}
	return 0;
}
/*-------------------------------------------------------------------------------------*/

