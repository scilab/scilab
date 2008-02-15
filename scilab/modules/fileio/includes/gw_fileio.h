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
int C2F(gw_fileio)(void);
/*--------------------------------------------------------------------------*/ 
int intsmeof __PARAMS((char *fname,unsigned long fname_len));
int intsmopen __PARAMS((char *fname,unsigned long fname_len));
int intsmclose __PARAMS((char *fname,unsigned long fname_len));
int intsmerror __PARAMS((char *fname,unsigned long fname_len));
int intsmputstr __PARAMS((char *fname,unsigned long fname_len));
int intsmput __PARAMS((char *fname,unsigned long fname_len));
int intsmget __PARAMS((char *fname,unsigned long fname_len));
int intsmgetstr __PARAMS((char *fname,unsigned long fname_len));
int intsmseek __PARAMS((char *fname,unsigned long fname_len));
int intsmtell __PARAMS((char *fname,unsigned long fname_len));
int intsmclearerr __PARAMS((char *fname,unsigned long fname_len));
int int_objfscanfMat __PARAMS((char *fname,unsigned long fname_len));
int int_objsprintf __PARAMS((char *fname,unsigned long fname_len));
int int_objnumTokens __PARAMS((char *fname,unsigned long fname_len));
int int_objfprintf __PARAMS((char *fname,unsigned long fname_len));
int int_objscanf __PARAMS((char *fname,unsigned long fname_len));
int int_objfprintfMat __PARAMS((char *fname,unsigned long fname_len));
int int_objsscanf __PARAMS((char *fname,unsigned long fname_len));
int int_objfscanf __PARAMS((char *fname,unsigned long fname_len));
int int_objprintf __PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_findfiles) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_maxfiles) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getdrives) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fullpath) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_deletefile) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_pathsep) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_filesep) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getlongpathname) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_getshortpathname) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_fileext) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_isdir) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_createdir) _PARAMS((char *fname,unsigned long fname_len));
int C2F(sci_removedir) _PARAMS((char *fname,unsigned long fname_len));
/*--------------------------------------------------------------------------*/ 
#endif /* __GW_FILEIO_H__ */
/*--------------------------------------------------------------------------*/
