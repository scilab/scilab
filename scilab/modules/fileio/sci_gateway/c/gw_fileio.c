/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_fileio.h"
#include "callFunctionFromGateway.h"
#include "MALLOC.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/ 
/*  interface function */
/*--------------------------------------------------------------------------*/ 
static gw_generic_table Tab[]={
	{sci_mopen, "mopen"},
	{sci_mputstr, "mputstr"},
	{sci_mclose, "mclose"},
	{sci_mput, "mput"},
	{sci_mget, "mget"},
	{sci_mgetstr, "mgetstr"},
	{sci_meof, "meof"},
	{sci_mseek, "mseek"},
	{sci_mtell, "mtell"},
	{sci_mclearerr, "mclearerr"},
	{sci_mfprintf,"mfprintf"},
	{sci_scanf,"mscanf"},
	{sci_fscanf,"mfscanf"},
	{sci_sscanf,"msscanf"},
	{sci_fscanfMat,"fscanfMat"},
	{sci_fprintfMat,"fprintfMat"},
	{sci_merror, "merror"},
	{sci_findfiles,"findfiles"},
	{sci_maxfiles,"maxfiles"},
	{sci_getdrives,"getdrives"},
	{sci_fullpath,"fullpath"},
	{sci_deletefile,"deletefile"},
	{sci_pathsep,"pathsep"},
	{sci_filesep,"filesep"},
	{sci_getlongpathname,"getlongpathname"},
	{sci_getshortpathname,"getshortpathname"},
	{sci_fileext,"fileext"},
	{sci_isdir,"isdir"},
	{sci_removedir,"removedir"},
	{sci_createdir,"createdir"},
	{sci_pwd,"pwd"},
	{sci_chdir,"chdir"},
	{sci_fileinfo,"fileinfo"},
	{sci_newest,"newest"},
	{sci_getrelativefilename,"getrelativefilename"},
	{sci_get_absolute_file_path,"get_absolute_file_path"},
	{sci_copyfile,"copyfile"},
	{sci_isfile,"isfile"},
	{sci_fileparts,"fileparts"},
	{sci_movefile,"movefile"},
	{sci_basename,"basename"},
	{sci_pathconvert,"pathconvert"},
	{sci_chdir,"cd"}
	};
/*--------------------------------------------------------------------------*/ 
int gw_fileio(void)
{
	Rhs = Max(0,Rhs);

	if(pvApiCtx == NULL)
	{
		pvApiCtx = (StrCtx*)MALLOC(sizeof(StrErr));
	}

	pvApiCtx->pstName = (char*)Tab[Fin-1].name;
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/ 

