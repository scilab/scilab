/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Sylvestre LEDRU
* Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "MALLOC.h"
#include "stack-c.h"
#include "localization.h"
#include "gw_localization.h"
#include "Scierror.h"
#include "strsubst.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
int sci_gettext(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,1);

	if ( (GetType(1) == sci_strings) )
	{
		int l1 = 0, n1 = 0, m1 = 0;
		char *tmpStr = NULL;

		char *msgid = NULL;
		char *TranslatedString = NULL;

		int revertStrsub = FALSE;

		GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
		msgid = cstk(l1);

		/* This stupid stuff is necessary because scilab is add slashes
		* and we need to remove them
		* An other solution might be to replace the string "\x" by it
		* real code 
		*/
		if (strchr(msgid, '\\') != NULL)
		{
			char *tmpStr1 = NULL;
			char *tmpStr2 = NULL;
			/* There is an \ in the string process to replace */

			/* We always have something from this functions because gettext
			* is returning the same string if it cannot find it */

			tmpStr1 = strsub(msgid, "\\n", "\n"); /* linefeed */
			tmpStr2 = strsub(tmpStr1, "\\t", "\t"); /* horizontal tab */
			if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

			tmpStr1 = strsub(tmpStr2, "\\r", "\r"); /* carriage return */
			if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}

			tmpStr2 = strsub(tmpStr1, "\\v", "\v"); /* vertical tab */
			if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

			tmpStr1 = strsub(tmpStr2, "\\f", "\f"); /* form feed */
			if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}

			tmpStr2 = strsub(tmpStr1, "\\\\", "\\"); /* backslash */
			if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

			tmpStr1 = strsub(tmpStr2, "\\\"", "\""); /* double quote */
			if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}

			revertStrsub = TRUE;
			tmpStr = strdup(tmpStr1);
			if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}
		}
		else
		{
			revertStrsub = FALSE;
			tmpStr = strdup(msgid);
		}

		TranslatedString = strdup(gettext(tmpStr));
		if (tmpStr) {FREE(tmpStr); tmpStr = NULL;}

		/* Add removed slashes */
		if (revertStrsub)
		{
			char *tmpStr1 = NULL;
			char *tmpStr2 = NULL;

			tmpStr1 = strsub(TranslatedString, "\\", "\\\\"); /* backslash */

			tmpStr2 = strsub(tmpStr1, "\f", "\\f"); /* form feed */
			if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

			tmpStr1 = strsub(tmpStr2, "\n", "\\n"); /* linefeed */
			if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}

			tmpStr2 = strsub(tmpStr1, "\t", "\\t"); /* horizontal tab */
			if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

			tmpStr1 = strsub(tmpStr2, "\r", "\\r"); /* carriage return */
			if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}

			tmpStr2 = strsub(tmpStr1, "\v", "\\v"); /* vertical tab */
			if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

			if (TranslatedString) {FREE(TranslatedString); TranslatedString = NULL;}
			TranslatedString = strdup(tmpStr2);
			if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}
		}

		n1 = 1;
		m1 = (int)strlen(TranslatedString);
		CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, &m1, &n1, &TranslatedString);
		LhsVar(1) = Rhs + 1;
		C2F(putlhsvar)();

		if (TranslatedString) {FREE(TranslatedString); TranslatedString = NULL;}
		return 0;
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
		return 0;
	}
}
/*--------------------------------------------------------------------------*/
