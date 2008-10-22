/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#ifndef __GW_FILEIO_H__
#define __INTFILEIO__
/*--------------------------------------------------------------------------*/ 
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
int gw_fileio(void);
/*--------------------------------------------------------------------------*/ 
int intsmeof(char *fname,unsigned long fname_len);
int intsmopen(char *fname,unsigned long fname_len);
int intsmclose(char *fname,unsigned long fname_len);
int intsmerror(char *fname,unsigned long fname_len);
int intsmputstr(char *fname,unsigned long fname_len);
int intsmput(char *fname,unsigned long fname_len);
int intsmget(char *fname,unsigned long fname_len);
int intsmgetstr(char *fname,unsigned long fname_len);
int intsmseek(char *fname,unsigned long fname_len);
int intsmtell(char *fname,unsigned long fname_len);
int intsmclearerr(char *fname,unsigned long fname_len);
int int_objfscanfMat(char *fname,unsigned long fname_len);
int int_objsprintf(char *fname,unsigned long fname_len);
int int_objnumTokens(char *fname,unsigned long fname_len);
int int_objfprintf(char *fname,unsigned long fname_len);
int int_objscanf(char *fname,unsigned long fname_len);
int int_objfprintfMat(char *fname,unsigned long fname_len);
int int_objsscanf(char *fname,unsigned long fname_len);
int int_objfscanf(char *fname,unsigned long fname_len);
int int_objprintf(char *fname,unsigned long fname_len);
int C2F(sci_findfiles)(char *fname,unsigned long fname_len);
int C2F(sci_maxfiles)(char *fname,unsigned long fname_len);
int C2F(sci_getdrives)(char *fname,unsigned long fname_len);
int C2F(sci_fullpath)(char *fname,unsigned long fname_len);
int C2F(sci_deletefile)(char *fname,unsigned long fname_len);
int C2F(sci_pathsep)(char *fname,unsigned long fname_len);
int C2F(sci_filesep)(char *fname,unsigned long fname_len);
int C2F(sci_getlongpathname)(char *fname,unsigned long fname_len);
int C2F(sci_getshortpathname)(char *fname,unsigned long fname_len);
int C2F(sci_fileext)(char *fname,unsigned long fname_len);
int C2F(sci_isdir)(char *fname,unsigned long fname_len);
int C2F(sci_createdir)(char *fname,unsigned long fname_len);
int C2F(sci_removedir)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /* __GW_FILEIO_H__ */
/*--------------------------------------------------------------------------*/
