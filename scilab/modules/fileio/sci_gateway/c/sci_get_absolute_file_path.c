/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*
 * Provides get_absolute_file_path to scilab
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_io.h"
#include "stack-c.h"
#include "PATH_MAX.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "getfiledesc.h"
#include "getfileinfo.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_get_absolute_file_path(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,1);

	if (GetType(1) != sci_strings)
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string.\n"),fname,1);
		return 0;
	}
	else
	{
		int n1,m1;
		char **parametersIN = NULL;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&parametersIN);

		if ( (m1 == 1) && (n1 == 1) )
		{
			if ( strcmp(parametersIN[0],"") == 0 ) /* */
			{
				Scierror(999,_("%s: Wrong value for input argument: no empty string expected.\n"),fname);
				freeArrayOfString(parametersIN,m1*n1);
				return 0;
			}
			else
			{
				int i = 0;
				int fdmax = 0;
				char fileNameFormList[PATH_MAX];
				char *absolute_file_path = NULL;
				char *filename = parametersIN[0];

				C2F(getfiledesc)(&fdmax);
				for(i=fdmax-1;i>=0;i--)
				{
					FILE fa;
					int swap2 = 0;
					int type = 0;
					int mode = 0;

					int lf = 0;
					int ierr = 0;

					int posBeginFileName = 0;

					C2F(getfileinfo)(&i, &fa, &swap2,&type,&mode,fileNameFormList,&lf,&ierr);
					if (ierr == 0)
					{
						posBeginFileName = (int)(strlen(fileNameFormList)-strlen(filename));

						if ( posBeginFileName > 0 )
						{
							char *cmptmp = NULL;
							cmptmp = &fileNameFormList[posBeginFileName];
							if ( strcmp(cmptmp,filename) == 0 )
							{	
								absolute_file_path=(char *)MALLOC(sizeof(char)*(strlen(fileNameFormList)+1));
								if (absolute_file_path)
								{
									strncpy(absolute_file_path,fileNameFormList,posBeginFileName);
									absolute_file_path[posBeginFileName]='\0';
									break;
								}
							}
						}
					}
				}

				if (!absolute_file_path) /* file not found in list of files opened by scilab */
				{
					Scierror(999,_("%s: The file %s is not opened in scilab.\n"),fname,filename);
					freeArrayOfString(parametersIN,m1*n1);
					return 0;
				}

				freeArrayOfString(parametersIN,m1*n1);

				n1=1;
				CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(absolute_file_path), &m1),&n1,&absolute_file_path);
				LhsVar(1)=Rhs+1;
				C2F(putlhsvar)();

				if (absolute_file_path){FREE(absolute_file_path);absolute_file_path=NULL;}
			}
		}
		else
		{
			freeArrayOfString(parametersIN,m1*n1);
			Scierror(999,_("%s: Wrong type for input argument #%d: A string expected.\n"),fname,1);
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/

