/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "gw_gui.h"
#include "stack-c.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "sciprint.h"
#include "scilabmode.h"
#include "localization.h"
#include "IsAScalar.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_ClipBoard)(char *fname,unsigned long l)
{
	static int l1,n1,m1;
	char *param1=NULL,*param2=NULL;

	Rhs=Max(0,Rhs);
	CheckRhs(0,2);
	CheckLhs(0,1);

	if ( getScilabMode() == SCILAB_STD )
	{
		if ( (Rhs == 1) && (GetType(1)==sci_strings) )
		{
			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			param1=cstk(l1);

			if ( ( strcmp(param1,"paste") == 0 ) || ( strcmp(param1,"pastespecial") == 0 ) )
			{
				char *output=NULL ;

				/* Get Text from clipboard */
				/* TO DO interface with java */

				if (output)
				{
					CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(output), &m1),&n1,&output);
					FREE(output);
					LhsVar(1)=Rhs+1;
				}
				else
				{
					m1=0;
					n1=0;
					l1=0;
					CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
					LhsVar(1)=Rhs+1;
				}

				C2F(putlhsvar)();	
				return 0;
			}
			else
			{
				Scierror(999,_("Unknown first input argument. Should be 'paste' or 'pastespecial'.\n"));
				return 0;
			}
		}

		if (Rhs == 2)
		{
			if ( (GetType(1)==sci_strings) && (GetType(2)==sci_strings) )
			{
				GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
				param1=cstk(l1);
				/* @TODO : should be remplaced by an enum */
				if ( strcmp(param1,"do") == 0 )
				{
					GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
					param2=cstk(l1);

					if ( strcmp(param2,"paste") == 0 )
					{
						/* @TODO interface with java */
						/* PASTE action */
					}
					else if ( strcmp(param2,"copy") == 0 )
					{
						/* @TODO interface with java */
						/* COPY action */
					}
					else if ( strcmp(param2,"empty") == 0 )
					{
						/* @TODO interface with java */
						/* EMPTY clipboard action */
					}
					else
					{
						Scierror(999,_("Incorrect second input argument. Should be 'do', 'paste', 'copy' or 'empty'.\n"));
						return 0;
					}

					m1=0;
					n1=0;
					l1=0;
					CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
					LhsVar(1)=Rhs+1;
					C2F(putlhsvar)();	
					return 0;
				}

				if ( strcmp(param1,"copy") == 0 )
				{
					char *TextToPutInClipboard=NULL;
					char **Str=NULL;
					
					GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);


					if ( (m1==1) && (n1==1) )
					{
						TextToPutInClipboard=Str[0];
						/* TO DO interface with java */
						/* put text in clipboard */
					}
					else
					{
						int i=0;
						int j=0;
						int l2=0;
						char *TextToSendInClipboard=NULL;
						int SizeofTextToSendInClipboard=0;
						char **buffer = (char**)MALLOC( (m1*n1)*sizeof(char *) );

						for (i=0; i<m1; i++) for (j=0; j<n1; j++) 
						{
							SizeofTextToSendInClipboard=SizeofTextToSendInClipboard+(int)strlen(Str[j*m1+i])+(int)strlen("\n");
							buffer[i*n1+j]=(char*)MALLOC(strlen(Str[j*m1+i])+1);
							strcpy(buffer[i*n1+j],Str[j*m1+i]);
						}

						TextToSendInClipboard=(char*)MALLOC( (SizeofTextToSendInClipboard)*sizeof(char) );
						strcpy(TextToSendInClipboard,"");

						for (i=0; i<m1; i++)
						{
							for (j=0; j<n1; j++) 
							{
								strcat(TextToSendInClipboard,buffer[l2++]);
								strcat(TextToSendInClipboard," ");
							}
							if ( i != (m1-1) ) strcat(TextToSendInClipboard,"\n");
						}

						/* TO DO interface with java */
						/* Put Text in clipboard */

						if (buffer) {FREE(buffer);buffer=NULL;}

						if(TextToSendInClipboard) {FREE(TextToSendInClipboard);TextToSendInClipboard=NULL;}
					}

					freeArrayOfString(Str,m1*n1);
					m1=0;
					n1=0;
					l1=0;
					CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
					LhsVar(1)=Rhs+1;
					C2F(putlhsvar)();	
					return 0;
				}
				else
				{
					Scierror(999,_("Incorrect input argument. See : help clipboard"));
					return 0;
				}
			}

			if ( (IsAScalar(1)) && (GetType(2)==sci_strings) )
			{
				GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
				param2=cstk(l1);

				if ( ( strcmp(param2,"EMF") == 0 ) || ( strcmp(param2,"DIB") == 0 ) )
				{
					int num_win=-2;
					GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
					num_win=*istk(l1);
					if (num_win>=0)
					{
						sciprint(_("Not yet implemented.\n"));
						{
/*
// 						struct BCG *ScilabGC=NULL;
// 						ScilabGC = getWindowXgcNumber (num_win);
// 
// 						if (ScilabGC)
// 						{
// 							if ( strcmp(param2,"EMF") == 0 )
// 							{
// 								CopyToClipboardEMF (ScilabGC);
// 							}
// 							else
// 							{
// 								CopyToClipboardBitmap (ScilabGC);
// 							}
*/
						/* TO DO interface with java */
						/* copy in clipboard EMF or DIB */
						

							m1=0;
							n1=0;
							l1=0;
							CreateVar(1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
							LhsVar(1)=1;
							C2F(putlhsvar)();	
							return 0;
						}
						//else
						//{
						//	Scierror(999,"Invalid Windows number.");
						//	return 0;
						//}
					}
					else
					{
						Scierror(999,_("Input argument must be >= 0."));
						return 0;
					}
				}
				else
				{
					Scierror(999,_("Second input argument must be 'EMF' or 'DIB'."));
					return 0;
				}
			}
		}
		else
		{
			Scierror(999,_("Incorrect argument. See : help clipboard"));
			return 0;	
		}
	}
	else
	{
		if ( (Rhs == 2) && (IsAScalar(1)) && (GetType(2)==sci_strings) )
		{
			GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
			param2=cstk(l1);
			if ( ( strcmp(param2,"EMF") == 0 ) || ( strcmp(param2,"DIB") == 0 ) )
			{
				int num_win=-2;
				GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
				num_win=*istk(l1);
				if (num_win>=0)
				{
/*
// 					struct BCG *ScilabGC=NULL;
// 					ScilabGC = getWindowXgcNumber (num_win);
// 
// 					if (ScilabGC)
// 					{
// 						if ( strcmp(param2,"EMF") == 0 )
// 						{
// 							CopyToClipboardEMF (ScilabGC);
// 						}
// 						else
// 						{
// 							CopyToClipboardBitmap (ScilabGC);
// 						}
*/
					/* TO DO interface with java */
					/* copy in clipboard EMF or DIB */
					{

					sciprint(_("Not yet implemented.\n"));

						m1=0;
						n1=0;
						l1=0;
						CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
						LhsVar(1)=Rhs+1;
						C2F(putlhsvar)();	
						return 0;
					}
					//else
					//{
					//	Scierror(999,"Invalid Windows number.");
					//	return 0;
					//}
				}
				else
				{
					Scierror(999,_("Input argument must be >= 0."));
					return 0;
				}
			}
			else
			{
				Scierror(999,_("Second input argument must be 'EMF' or 'DIB'."));
				return 0;
			}
		}
		else
		{
			Scierror(999,_("Only on Windows Mode.\n"));
			return 0;
		}
		Scierror(999,_("Only on Windows Mode.\n"));
		return 0;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
