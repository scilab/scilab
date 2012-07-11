/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - INRIA - Sylvestre LEDRU
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* Copyright (C) 2010 - DIGITEO - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "funcmanager.hxx"
#include "localization_gw.hxx"
#include "function.hxx"
#include "string.hxx"

extern "C"
{
#include <string.h>
#include <stdlib.h>
#include "MALLOC.h"
#include "localization.h"
#include "Scierror.h"
#include "os_wcsdup.h"
#include "strsubst.h"
}

wchar_t* ScilabStringToCString(wchar_t *_pwstIn, bool* _pbIsConverted);
wchar_t* CStringToScilabString(wchar_t *_pwstIn);
void SwitchString(wchar_t** _pwst1, wchar_t** _pwst2);
/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_gettext(typed_list &in, int _piRetCount, typed_list &out)
{
    if(in.size() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong number of input arguments: at least %d expected.\n"), L"gettext", 1);
        return Function::Error;
    }

    if(in[0]->isString() == false || in[0]->getAs<types::String>()->getSize() != 1)
    {
        ScierrorW(999, _W("%ls: Wrong type for input argument #%d: A string expected.\n"), L"gettext" ,1);
        return Function::Error;
    }


    bool bConveted      = false;
    wchar_t* pwstCIn    = ScilabStringToCString(in[0]->getAs<types::String>()->get()[0], &bConveted);
    char* pstCIn        = wide_string_to_UTF8(pwstCIn);
    wchar_t* pwstOut    = _W(pstCIn);
    if(bConveted)
    {
        wchar_t* pwstTemp    = CStringToScilabString(pwstOut);
        SwitchString(&pwstOut, &pwstTemp);
    }

    String* pOut        = new String(1,1);
    pOut->set(0, pwstOut);
    out.push_back(pOut);

    FREE(pwstCIn);
    FREE(pstCIn);
    FREE(pwstOut);


    return Function::OK;
 //   CheckRhs(1,1);
	//CheckLhs(1,1);

	//if ( (GetType(1) == sci_strings) )
	//{
	//	int l1 = 0, n1 = 0, m1 = 0;
	//	char *tmpStr = NULL;

	//	char *msgid = NULL;
	//	char *TranslatedString = NULL;

	//	int revertStrsub = FALSE;

	//	GetRhsVar(1,STRING_DATATYPE, &m1, &n1, &l1);
	//	msgid = cstk(l1);

	//	/* This stupid stuff is necessary because scilab is add slashes
	//	* and we need to remove them
	//	* An other solution might be to replace the string "\x" by it
	//	* real code
	//	*/
	//	if (strchr(msgid, '\\') != NULL)
	//	{
	//		char *tmpStr1 = NULL;
	//		char *tmpStr2 = NULL;
	//		/* There is an \ in the string process to replace */

	//		/* We always have something from this functions because gettext
	//		* is returning the same string if it cannot find it */

	//		tmpStr1 = strsub(msgid, "\\n", "\n"); /* linefeed */
	//		tmpStr2 = strsub(tmpStr1, "\\t", "\t"); /* horizontal tab */
	//		if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

	//		tmpStr1 = strsub(tmpStr2, "\\r", "\r"); /* carriage return */
	//		if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}

	//		tmpStr2 = strsub(tmpStr1, "\\v", "\v"); /* vertical tab */
	//		if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

	//		tmpStr1 = strsub(tmpStr2, "\\f", "\f"); /* form feed */
	//		if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}

	//		tmpStr2 = strsub(tmpStr1, "\\\\", "\\"); /* backslash */
	//		if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

	//		tmpStr1 = strsub(tmpStr2, "\\\"", "\""); /* double quote */
	//		if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}

	//		revertStrsub = TRUE;
	//		tmpStr = os_strdup(tmpStr1);
	//		if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}
	//	}
	//	else
	//	{
	//		revertStrsub = FALSE;
	//		tmpStr = os_strdup(msgid);
	//	}

	//	TranslatedString = os_strdup(gettext(tmpStr));
	//	if (tmpStr) {FREE(tmpStr); tmpStr = NULL;}

	//	/* Add removed slashes */
	//	if (revertStrsub)
	//	{
	//		char *tmpStr1 = NULL;
	//		char *tmpStr2 = NULL;

	//		tmpStr1 = strsub(TranslatedString, "\\", "\\\\"); /* backslash */

	//		tmpStr2 = strsub(tmpStr1, "\f", "\\f"); /* form feed */
	//		if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

	//		tmpStr1 = strsub(tmpStr2, "\n", "\\n"); /* linefeed */
	//		if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}

	//		tmpStr2 = strsub(tmpStr1, "\t", "\\t"); /* horizontal tab */
	//		if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

	//		tmpStr1 = strsub(tmpStr2, "\r", "\\r"); /* carriage return */
	//		if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}

	//		tmpStr2 = strsub(tmpStr1, "\v", "\\v"); /* vertical tab */
	//		if (tmpStr1) {FREE(tmpStr1); tmpStr1 = NULL;}

	//		if (TranslatedString) {FREE(TranslatedString); TranslatedString = NULL;}
	//		TranslatedString = os_strdup(tmpStr2);
	//		if (tmpStr2) {FREE(tmpStr2); tmpStr2 = NULL;}
	//	}

	//	n1 = 1;
	//	m1 = (int)strlen(TranslatedString);
	//	CreateVarFromPtr(Rhs + 1, STRING_DATATYPE, &m1, &n1, &TranslatedString);
	//	LhsVar(1) = Rhs + 1;
	//	C2F(putlhsvar)();

	//	if (TranslatedString) {FREE(TranslatedString); TranslatedString = NULL;}
	//	return 0;
	//}
	//else
	//{
	//	Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), fname, 1);
	//	return 0;
	//}
}
/*--------------------------------------------------------------------------*/
wchar_t* ScilabStringToCString(wchar_t *_pwstIn, bool* _pbIsConverted)
{
    if(wcschr(_pwstIn, L'\\'))
    {// '\\' is present so find specials characters
        //replace "\\n" by "\n"
        wchar_t* pwstTemp1 = NULL;
        wchar_t* pwstTemp2 = NULL;

        //replace "\\n" by "\n"
        pwstTemp1 = wcssub(_pwstIn, L"\\n", L"\n");

        //replace "\\t" by "\t"
        pwstTemp2 = wcssub(pwstTemp1, L"\\t", L"\t");
        SwitchString(&pwstTemp1, &pwstTemp2);

        //replace "\\r" by "\r"
        pwstTemp2 = wcssub(pwstTemp1, L"\\r", L"\r");
        SwitchString(&pwstTemp1, &pwstTemp2);

        //replace "\\v" by "\v"
        pwstTemp2 = wcssub(pwstTemp1, L"\\v", L"\v");
        SwitchString(&pwstTemp1, &pwstTemp2);

        //replace "\\f" by "\f"
        pwstTemp2 = wcssub(pwstTemp1, L"\\f", L"\f");
        SwitchString(&pwstTemp1, &pwstTemp2);

        //replace "\\\\" by "\\"
        pwstTemp2 = wcssub(pwstTemp1, L"\\\\", L"\\");
        SwitchString(&pwstTemp1, &pwstTemp2);

        //replace "\\f" by "\f"
        pwstTemp2 = wcssub(pwstTemp1, L"\\\"", L"\"");
        SwitchString(&pwstTemp1, &pwstTemp2);

        *_pbIsConverted = true;
        return pwstTemp1;
    }
    else
    {
        *_pbIsConverted = false;
        return os_wcsdup(_pwstIn);
    }
}
/*--------------------------------------------------------------------------*/
wchar_t* CStringToScilabString(wchar_t *_pwstIn)
{
    wchar_t* pwstOut = NULL;

    wchar_t* pwstTemp1 = NULL;
    wchar_t* pwstTemp2 = NULL;

    //replace "\\" by "\\\\"
    pwstTemp1 = wcssub(_pwstIn, L"\\", L"\\\\");

    //replace "\f" by "\\f"
    pwstTemp2 = wcssub(pwstTemp1, L"\f", L"\\f");
    SwitchString(&pwstTemp1, &pwstTemp2);

    //replace "\n" by "\\n"
    pwstTemp2 = wcssub(pwstTemp1, L"\n", L"\\n");
    SwitchString(&pwstTemp1, &pwstTemp2);

    //replace "\t" by "\\t"
    pwstTemp2 = wcssub(pwstTemp1, L"\t", L"\\t");
    SwitchString(&pwstTemp1, &pwstTemp2);

    //replace "\r" by "\\r"
    pwstTemp2 = wcssub(pwstTemp1, L"\r", L"\\r");
    SwitchString(&pwstTemp1, &pwstTemp2);

    //replace "\v" by "\\v"
    pwstTemp2 = wcssub(pwstTemp1, L"\v", L"\\v");
    SwitchString(&pwstTemp1, &pwstTemp2);

    return pwstTemp1;
}
/*--------------------------------------------------------------------------*/
void SwitchString(wchar_t** _pwst1, wchar_t** _pwst2)
{
    if(_pwst2 && *_pwst2)
    {
        FREE(*_pwst1);
        *_pwst1 = *_pwst2;
        *_pwst2 = NULL;
    }
}
