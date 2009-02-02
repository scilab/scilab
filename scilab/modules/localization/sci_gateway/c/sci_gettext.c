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
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_gettext)(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,1);

	if ( (GetType(1) == sci_strings) )
	{
		static int l1,n1,m1;

		char *msgid=NULL;
		char *TranslatedString=NULL;

		int revertStrsub = FALSE;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		msgid=cstk(l1);

		/* This stupid stuff is necessary because scilab is add slashes
		* and we need to remove them
		* An other solution might be to replace the string "\x" by it
		* real code 
		*/
		if (strchr(msgid, '\\')!=NULL){
			/* There is an \ in the string process to replace */

			/* We always have something from this functions because gettext
			* is returning the same string if it cannot find it */

			msgid=strsub(msgid, "\\n", "\n"); /* linefeed */
			msgid=strsub(msgid, "\\t", "\t"); /* horizontal tab */
			msgid=strsub(msgid, "\\r", "\r"); /* carriage return */
			msgid=strsub(msgid, "\\v", "\v"); /* vertical tab */
			msgid=strsub(msgid, "\\f", "\f"); /* form feed */
			msgid=strsub(msgid, "\\\\", "\\"); /* backslash */
			msgid=strsub(msgid, "\\\"", "\""); /* double quote */

			revertStrsub = TRUE;
		}
		TranslatedString=gettext(msgid);

		/* Add removed slashes */
		if (revertStrsub)
		{
			TranslatedString=strsub(TranslatedString, "\\", "\\\\"); /* backslash */
			TranslatedString=strsub(TranslatedString, "\"", "\\\""); /* double quote */
			TranslatedString=strsub(TranslatedString, "\n", "\\n"); /* linefeed */
			TranslatedString=strsub(TranslatedString, "\t", "\\t"); /* horizontal tab */
			TranslatedString=strsub(TranslatedString, "\r", "\\r"); /* carriage return */
			TranslatedString=strsub(TranslatedString, "\v", "\\v"); /* vertical tab */
			TranslatedString=strsub(TranslatedString, "\f", "\\f"); /* form feed */
		}

		n1=1;
		CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(TranslatedString), &m1),&n1,&TranslatedString);
		LhsVar(1)=Rhs+1;
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
