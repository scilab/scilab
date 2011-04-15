/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
	{NULL, ""}, //mopen
	{NULL, ""}, //mputstr
	{NULL, ""}, //mclose
	{NULL, ""}, //mput
	{NULL, ""}, //mget
	{NULL, ""}, //mgetstr
	{NULL, ""}, //meof
	{NULL, ""}, //mseek
	{NULL, ""}, //mtell
	{sci_mclearerr, "mclearerr"},
	{sci_mfprintf,"mfprintf"}, //
	{sci_scanf,"mscanf"}, //
	{sci_fscanf,"mfscanf"}, //
	{sci_sscanf,"msscanf"}, //
	{sci_fscanfMat,"fscanfMat"},
	{sci_fprintfMat,"fprintfMat"},
	{sci_merror, "merror"},
	{NULL, ""}, //findfiles
	{sci_maxfiles,"maxfiles"},
	{sci_getdrives,"getdrives"},
	{NULL, ""}, //fullpath
	{NULL, ""}, //deletefile
	{sci_pathsep,"pathsep"},
	{NULL, ""}, //filesep
	{NULL, ""}, //getlongpathname
	{NULL, ""}, //getshortpathname
	{NULL, ""}, //fileext
	{NULL, ""}, //isdir
	{NULL, ""}, //removedir
	{NULL, ""}, //createdir
	{NULL, ""}, //pwd
	{NULL, ""}, //chdir
	{NULL, ""}, //fileinfo
	{NULL, ""}, //newest
	{sci_getrelativefilename,"getrelativefilename"},
	{sci_get_absolute_file_path,"get_absolute_file_path"},
	{NULL, ""}, //copyfile
	{NULL, ""}, //isfile
	{NULL, ""}, //fileparts
	{NULL, ""}, //movefile
	{NULL, ""}, //basename
	{NULL, ""}, //pathconvert
	{NULL, ""}  //cd
	};
/*--------------------------------------------------------------------------*/
int gw_fileio(void)
{
    Rhs = Max(0,Rhs);

    callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
    return 0;
}
/*--------------------------------------------------------------------------*/

