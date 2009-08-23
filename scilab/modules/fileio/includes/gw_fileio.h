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
#define __GW_FILEIO_H__

#include "api_common.h"
/*--------------------------------------------------------------------------*/ 
int gw_fileio(void);
/*--------------------------------------------------------------------------*/ 
int sci_meof(char *fname,unsigned long fname_len);
int sci_mopen(char *fname,unsigned long fname_len);
int sci_mclose(char *fname,unsigned long fname_len);
int sci_merror(char *fname,unsigned long fname_len);
int sci_mputstr(char *fname,unsigned long fname_len);
int sci_mput(char *fname,unsigned long fname_len);
int sci_mget(char *fname,unsigned long fname_len);
int sci_mgetstr(char *fname,unsigned long fname_len);
int sci_mseek(char *fname,unsigned long fname_len);
int sci_mtell(char *fname,unsigned long fname_len);
int sci_mclearerr(char *fname,unsigned long fname_len);
int sci_fscanfMat(char *fname,unsigned long fname_len);
int sci_mfprintf(char *fname,unsigned long fname_len);
int sci_scanf(char *fname,unsigned long fname_len);
int sci_fprintfMat(char *fname,unsigned long fname_len);
int sci_sscanf(char *fname,unsigned long fname_len);
int sci_fscanf(char *fname,unsigned long fname_len);
int sci_findfiles(char *fname,unsigned long fname_len);
int sci_maxfiles(char *fname,unsigned long fname_len);
int sci_getdrives(char *fname,unsigned long fname_len);
int sci_fullpath(char *fname,unsigned long fname_len);
int sci_deletefile(char *fname,unsigned long fname_len);
int sci_pathsep(char *fname,unsigned long fname_len);
int sci_filesep(char *fname,unsigned long fname_len);
int sci_getlongpathname(char *fname,unsigned long fname_len);
int sci_getshortpathname(char *fname,unsigned long fname_len);
int sci_fileext(char *fname,unsigned long fname_len);
int sci_isdir(char *fname,unsigned long fname_len);
int sci_createdir(char *fname,unsigned long fname_len);
int sci_removedir(char *fname,unsigned long fname_len);
int sci_chdir(char *fname,unsigned long fname_len);
int sci_pwd(char *fname,unsigned long fname_len);
int sci_fileinfo(char *fname,unsigned long fname_len);
int sci_newest(char *fname,unsigned long fname_len);
int sci_getrelativefilename(char *fname, unsigned long l);
int sci_get_absolute_file_path(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /* __GW_FILEIO_H__ */
/*--------------------------------------------------------------------------*/
