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
#include "dynlib_fileio.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/ 
FILEIO_IMPEXP int gw_fileio(void);
/*--------------------------------------------------------------------------*/ 
FILEIO_IMPEXP int sci_meof(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_mopen(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_mclose(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_merror(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_mputstr(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_mput(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_mget(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_mgetstr(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_mseek(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_mtell(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_mclearerr(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_fscanfMat(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_mfprintf(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_scanf(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_fprintfMat(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_sscanf(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_fscanf(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_findfiles(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_maxfiles(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_getdrives(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_fullpath(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_deletefile(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_pathsep(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_filesep(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_getlongpathname(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_getshortpathname(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_fileext(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_isdir(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_createdir(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_removedir(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_chdir(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_pwd(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_fileinfo(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_newest(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_getrelativefilename(char *fname, unsigned long l);
FILEIO_IMPEXP int sci_get_absolute_file_path(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_copyfile(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_isfile(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_fileparts(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_movefile(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_basename(char *fname,unsigned long fname_len);
FILEIO_IMPEXP int sci_pathconvert(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/ 
#endif /* __GW_FILEIO_H__ */
/*--------------------------------------------------------------------------*/
