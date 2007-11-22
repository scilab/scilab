/*------------------------------------------------------------------------*/
/* File: sci_strcat1.c                                                    */
/* Copyright INRIA 2007                                                   */
/* @Authors : Cong Wu                                                     */
/* desc : catenate character strings                                      */
/* Examples: strcat(string(1:10),',')                                    */
/*                                                                        */
/*------------------------------------------------------------------------*/
#include <string.h>
#include "gw_string.h"
#include "stack-c.h"
#include "machine.h"
#include "MALLOC.h"
#include "strcatfunction.h"
#include "Scierror.h"
#include "get_length.h"
/*-------------------------------------------------------------------------------------*/
static int sci_strcat_three_rhs(char *fname);
/*-------------------------------------------------------------------------------------*/
int C2F(sci_strcat) _PARAMS((char *fname,unsigned long fname_len))
{
	/* the two input arguments */
	char **Input_String_One = NULL;
	char **Input_String_Two = NULL; 

	int Row_One = 0,Col_One = 0,Row_Two = 0,Col_Two = 0,Number_Input_One = 0,Number_Input_Two = 0,numRow = 0,numCol = 0,i = 0;
	int length = 0;
	char **Output_StringMatrix = NULL;
	int Type_One = VarType(1);
	int Type_Two = VarType(2);
	int lr = 0;

	CheckRhs(1,3);
	CheckLhs(1,1);

	/* !!! This feature is not documented but used !!! */
	/* TO DO : extend help */

	if (Rhs == 3) 
	{
		sci_strcat_three_rhs(fname);
		return 0;
	}

    if (Rhs >= 1) 
	{
		switch (Type_One) 
		{
			case sci_strings :
				GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_String_One);
				/*@ the first argument*/
				Number_Input_One = Row_One*Col_One;   
			break;
			default :
				Scierror(999,"%s : first argument has a wrong type, expecting scalar or string matrix.\n",fname);
			return 0;
		} 	
	}
	if (Rhs == 1)
	{
		for (i = 0; i< Number_Input_One; i++)    
		{  
			length = length + (int)strlen(Input_String_One[i]);
		}

	    Output_StringMatrix = (char**)MALLOC(sizeof(char*));
		if (Output_StringMatrix)
		{
			Output_StringMatrix[0] = (char*)MALLOC(sizeof(char)*(length+1));
			if (Output_StringMatrix[0])
			{
				strcpy(Output_StringMatrix[0],Input_String_One[0]); 
				for (i=1; i<Number_Input_One;i++)
				{
					strcat(Output_StringMatrix[0] ,Input_String_One[i]);
				}
				strcat(Output_StringMatrix[0] ,"\0");
			}
			else
			{
				if (Output_StringMatrix) { FREE(Output_StringMatrix); Output_StringMatrix = NULL; }
				Scierror(999,"Error memory allocation.\n");
				return 0;
			}
		}
		else
		{
			
			Scierror(999,"Error memory allocation.\n");
			return 0;
		}
	}

    if (Rhs ==2 )
	{
		switch (Type_Two) 
		{
			case sci_strings :
				GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&Row_Two,&Col_Two,&Input_String_Two);
				/*@ the second argument*/
				Number_Input_Two = Row_Two*Col_Two; 
			break;

			default :
				Scierror(999,"%s : second argument has a wrong type, expecting scalar or string matrix.\n",fname);
				return 0;
			break;
		} 
	    length = Get_length(Input_String_One,Input_String_Two,Number_Input_One);
	}

	
    if (Number_Input_One == 1 ) 
	{
	   Output_StringMatrix = (char**)MALLOC(sizeof(char*));
	   if (Output_StringMatrix)
	   {
		   if (strlen(Input_String_One[0]) != 0)   Output_StringMatrix[0]=(char*)MALLOC(sizeof(char)*(strlen(Input_String_One[0])));
		   else Output_StringMatrix[0]=(char*)MALLOC(sizeof(char));

		   strcpy(Output_StringMatrix[0],Input_String_One[0]); 
	   }
	   else
	   {
		   Scierror(999,"Error memory allocation.\n");
		   return 0;
	   }
	}
	else
	{
		if (Rhs >=2)  
		{
			Output_StringMatrix = (char**)MALLOC(sizeof(char*));
			Output_StringMatrix[0] = (char*)MALLOC(sizeof(char)*(length));

			if (Output_StringMatrix[0]) 
			{
				/*Put the first Input_string and the second together*/
				strcpy(Output_StringMatrix[0] , Input_String_One[0]);  
				strcat(Output_StringMatrix[0] , Input_String_Two[0]);

				/* Main algorithm */
				if (!strcatfunction(Input_String_One,Input_String_Two,Number_Input_One,Output_StringMatrix))
				{  /*2th argument is an empty string*/
				    strcpy(Output_StringMatrix[0],Input_String_One[0]); 
					for (i=1; i<Number_Input_One;i++)
					{
						strcat(Output_StringMatrix[0] ,Input_String_One[i]);
					}
					strcat(Output_StringMatrix[0] ,"\0"); 

				
				}
			}
			else
			{
				FREE(Output_StringMatrix);
				Output_StringMatrix = NULL;
				Scierror(999, "2th argument must not be an empty string.\n");
				return 0;
			}
		}
   }

	/* put result on scilab stack */
	numRow   = 1 ;
	numCol   = (int)strlen(Output_StringMatrix[0]);
		
    CreateVar(Rhs+1,STRING_DATATYPE, &numRow,&numCol , &lr);
    strcpy(cstk(lr),Output_StringMatrix[0]);
	LhsVar(1) = Rhs+1 ;
	C2F(putlhsvar)();

	/* free pointers */
	if (Output_StringMatrix)
	{
		if (Output_StringMatrix[0]) { FREE(Output_StringMatrix[0]); Output_StringMatrix[0]=NULL;}
		FREE(Output_StringMatrix); Output_StringMatrix=NULL; 
	}
	
	return 0;
}
/*-------------------------------------------------------------------------------------*/
static int sci_strcat_three_rhs(char *fname)
{ 
	char typ = NULL;
	char **Input_String_One,**Output_String;
	static char def_sep[] ="";
	char *Input_String_Two = def_sep;
	static int un=1;
	int Row_One,Col_One,mn,i,j,k,Row_Two,Col_Two,l2,Row_Three,Col_Three,l3,nchars=0;

	switch ( VarType(1)) 
	{
	case sci_strings :
	  GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&Row_One,&Col_One,&Input_String_One);
	  mn = Row_One*Col_One;  
	  if (Rhs >= 2) {
		  GetRhsVar(2,STRING_DATATYPE,&Row_Two,&Col_Two,&l2);
		  Input_String_Two = cstk(l2);
	  }
	  if (Rhs >= 3) {
		  GetRhsVar(3,STRING_DATATYPE,&Row_Three,&Col_Three,&l3);
		  if ( Row_Three*Col_Three != 0) 
			  typ = cstk(l3)[0];
		  if (typ != COL && typ != ROW ) {
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
		  break;
	  case COL: 
		  /* return a column matrix */ 
		  if ( (Output_String = MALLOC((Row_One+1)*sizeof(char *)))==NULL) 
		  {
			  Scierror(999,"%s: Out of memory\r\n",fname);
			  return 0;
		  }
		  Output_String[Row_One]=NULL;
		  for (i= 0 ; i < Row_One ; i++) {
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
		  for (j= 0 ; j < Col_One ; j++) {
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
