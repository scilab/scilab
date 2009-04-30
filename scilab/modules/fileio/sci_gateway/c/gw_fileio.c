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
	{sci_printf,"mprintf"},
	{sci_fprintf,"mfprintf"},
	{sci_sprintf,"msprintf"},
	{sci_scanf,"mscanf"},
	{sci_fscanf,"mfscanf"},
	{sci_sscanf,"msscanf"},
	{sci_fscanfMat,"fscanfMat"},
	{sci_fprintfMat,"fprintfMat"},
	{sci_numTokens,"NumTokens"},
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
	{sci_getcwd,"getcwd"},
	{sci_chdir,"chdir"},
	{sci_fileinfo,"fileinfo"}
	};
/*--------------------------------------------------------------------------*/ 
int gw_fileio(void)
{
	Rhs = Max(0,Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/ 

