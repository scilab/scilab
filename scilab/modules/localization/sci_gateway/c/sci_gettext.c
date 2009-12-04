/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "MALLOC.h"
#include "stack-c.h"
#include "localization.h"
#include "gw_localization.h"
#include "Scierror.h"
#include "strsubst.h"
/*--------------------------------------------------------------------------*/
int sci_gettext(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,1);

	if ( (GetType(1) == sci_strings) )
		{
			static int l1,n1,m1;

			char *msgid1=NULL;
			char *msgid2=NULL;
			char *TranslatedString1=NULL;
			char *TranslatedString2=NULL;
                        int revertStrsub = FALSE;
		                        
			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			msgid1=cstk(l1);

			/* This stupid stuff is necessary because scilab is add slashes
			 * and we need to remove them
			 * An other solution might be to replace the string "\x" by it
			 * real code 
			*/
			if (strchr(msgid1, '\\')!=NULL){
				/* There is an \ in the string process to replace */
				
				/* We always have something from this functions because gettext
				 * is returning the same string if it cannot find it */

			        /* the Scilab strsub function allocates memory */

				msgid2=strsub(msgid1, "\\n", "\n");                /* linefeed */
				msgid1=strsub(msgid2, "\\t", "\t");  FREE(msgid2); /* horizontal tab */
				msgid2=strsub(msgid1, "\\r", "\r");  FREE(msgid1); /* carriage return */
				msgid1=strsub(msgid2, "\\v", "\v");  FREE(msgid2); /* vertical tab */
				msgid2=strsub(msgid1, "\\f", "\f");  FREE(msgid1); /* form feed */
				msgid1=strsub(msgid2, "\\\\", "\\"); FREE(msgid2); /* backslash */
				msgid2=strsub(msgid1, "\\\"", "\""); FREE(msgid1); /* double quote */
				msgid1=msgid2;

                                revertStrsub = TRUE;
			}
			TranslatedString1=gettext(msgid1);

                        /* Add removed slashes */
                        if (revertStrsub)
                          {
                            TranslatedString2=strsub(TranslatedString1, "\\", "\\\\"); /* backslash */
                            TranslatedString1=strsub(TranslatedString2, "\"", "\\\""); FREE(TranslatedString2); /* double quote */
                            TranslatedString2=strsub(TranslatedString1, "\n", "\\n");  FREE(TranslatedString1); /* linefeed */
                            TranslatedString1=strsub(TranslatedString2, "\t", "\\t");  FREE(TranslatedString2); /* horizontal tab */
                            TranslatedString2=strsub(TranslatedString1, "\r", "\\r");  FREE(TranslatedString1); /* carriage return */
                            TranslatedString1=strsub(TranslatedString2, "\v", "\\v");  FREE(TranslatedString2); /* vertical tab */
                            TranslatedString2=strsub(TranslatedString1, "\f", "\\f");  FREE(TranslatedString1); /* form feed */
			    TranslatedString1=TranslatedString2;
                          }

			n1=1;
			CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(TranslatedString1), &m1),&n1,&TranslatedString1);
			LhsVar(1)=Rhs+1;
			if (revertStrsub)
			{
				FREE(msgid1);
				FREE(TranslatedString1);
			}
			C2F(putlhsvar)();
			return 0;
		}
	else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname,1);
			return 0;
		}
}
/*--------------------------------------------------------------------------*/
