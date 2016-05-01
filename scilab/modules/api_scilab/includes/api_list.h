/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Scilab Enterprises - Antoine ELIAS
*
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
*/

#ifndef __LIST_API__
#define __LIST_API__

#if !defined(__INTERNAL_API_SCILAB__)
#error Do not include api_list.h. Include api_scilab.h instead.
#endif

#ifdef __cplusplus
extern "C" {
#endif
#define scilab_isDefined            API_PROTO(isDefined)
#define scilab_isUndefined          API_PROTO(isUndefined)
#define scilab_createList           API_PROTO(createList)
#define scilab_createTList          API_PROTO(createTList)
#define scilab_createMList          API_PROTO(createMList)
#define scilab_getListItem          API_PROTO(getListItem)
#define scilab_setListItem          API_PROTO(setListItem)
#define scilab_getTListFieldNames   API_PROTO(getTListFieldNames)
#define scilab_getTListField        API_PROTO(getTListField)
#define scilab_setTListField        API_PROTO(setTListField)
#define scilab_getMListFieldNames   API_PROTO(getMTListFieldNames)
#define scilab_getMListField        API_PROTO(getMListField)
#define scilab_setMListField        API_PROTO(setMListField)
#define scilab_appendToList         API_PROTO(appendToList)

int API_PROTO(isDefined)(scilabEnv env, scilabVar var, int index);
int API_PROTO(isUndefined)(scilabEnv env, scilabVar var, int index);

scilabVar API_PROTO(createList)(scilabEnv env);
scilabVar API_PROTO(createTList)(scilabEnv env, const wchar_t* type);
scilabVar API_PROTO(createMList)(scilabEnv env, const wchar_t* type);

scilabVar API_PROTO(getListItem)(scilabEnv env, scilabVar var, int index);
scilabStatus API_PROTO(setListItem)(scilabEnv env, scilabVar var, int pos, scilabVar val);

int API_PROTO(getTListFieldNames)(scilabEnv env, scilabVar var, wchar_t*** fieldnames);
scilabVar API_PROTO(getTListField)(scilabEnv env, scilabVar var, const wchar_t* field);
scilabStatus API_PROTO(setTListField)(scilabEnv env, scilabVar var, const wchar_t* field, scilabVar val);

int API_PROTO(getMListFieldNames)(scilabEnv env, scilabVar var, wchar_t*** fieldnames);
scilabVar API_PROTO(getMListField)(scilabEnv env, scilabVar var, const wchar_t* field);
scilabStatus API_PROTO(setMListField)(scilabEnv env, scilabVar var, const wchar_t* field, scilabVar val);

scilabStatus API_PROTO(appendToList)(scilabEnv env, scilabVar var, scilabVar val);
#ifdef __cplusplus
}
#endif
#endif /* __LIST_API__ */
