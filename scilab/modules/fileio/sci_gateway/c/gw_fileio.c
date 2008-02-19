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
	{ intsmopen, "mopen"},
	{ intsmputstr, "mputstr"},
	{ intsmclose, "mclose"},
	{ intsmput, "mput"},
	{ intsmget, "mget"},
	{ intsmgetstr, "mgetstr"},
	{ intsmeof, "meof"},
	{ intsmseek, "mseek"},
	{ intsmtell, "mtell"},
	{ intsmclearerr, "mclearerr"},
	{int_objprintf,"mprintf"},
	{int_objfprintf,"mfprintf"},
	{int_objsprintf,"msprintf"},
	{int_objscanf,"mscanf"},
	{int_objfscanf,"mfscanf"},
	{int_objsscanf,"msscanf"},
	{int_objfscanfMat,"fscanfMat"},
	{int_objfprintfMat,"fprintfMat"},
	{int_objnumTokens,"NumTokens"},
	{intsmerror, "merror"},
	{C2F(sci_findfiles),"findfiles"},
	{C2F(sci_maxfiles),"maxfiles"},
	{C2F(sci_getdrives),"getdrives"},
	{C2F(sci_fullpath),"fullpath"},
	{C2F(sci_deletefile),"deletefile"},
	{C2F(sci_pathsep),"pathsep"},
	{C2F(sci_filesep),"filesep"},
	{C2F(sci_getlongpathname),"getlongpathname"},
	{C2F(sci_getshortpathname),"getshortpathname"},
	{C2F(sci_fileext),"fileext"},
	{C2F(sci_isdir),"isdir"},
	{C2F(sci_removedir),"removedir"},
	{C2F(sci_createdir),"createdir"}
	};
/*--------------------------------------------------------------------------*/ 
int gw_fileio(void)
{
	Rhs=Max(0,Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/ 

