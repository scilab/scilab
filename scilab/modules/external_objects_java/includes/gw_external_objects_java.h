/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 * Copyright (C) 2013 - Scilab Enterprises - Sylvestre Ledru
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_EXTERNAL_OBJECTS_H__
#define __GW_EXTERNAL_OBJECTS_H__
/*--------------------------------------------------------------------------*/
#include "dynlib_external_objects_java_scilab.h"
/*--------------------------------------------------------------------------*/
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int gw_external_objects_java(void);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jimport(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jgetclassname(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jwrap(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jgetmethods(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jgetfields(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jgetfield(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jcompile(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jnewInstance(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jremove(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jexists(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_junwrap(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_junwraprem(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jenableTrace(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jdisableTrace(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jdeff(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jvoid(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jarray(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jcast(char *fname, unsigned long fname_len);
EXTERNAL_OBJECTS_JAVA_SCILAB_IMPEXP int sci_jallowClassReloading(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_EXTERNAL_OBJECTS_H__ */
/*--------------------------------------------------------------------------*/
