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
#include "api_scilab.h"
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
	{NULL, ""}, //mclearerr
	{NULL, ""}, //mfprintf *
	{NULL, ""}, //mscanf *
	{NULL, ""}, //mfscanf *
	{NULL, ""}, //msscanf *
	{NULL,"" }, //fscanfMat
	{NULL,"" }, //fprintfMat
	{NULL, ""}, //merror
	{NULL, ""}, //findfiles
	{NULL, ""}, //maxfiles => deprecated (Scilab 5.4)
	{NULL, ""}, //getdrives
	{NULL, ""}, //fullpath
	{NULL, ""}, //deletefile
	{NULL, ""}, //pathsep
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
	{NULL, ""}, //getrelativefilename
	{NULL, ""}, //get_absolute_file_path
	{NULL, ""}, //copyfile
	{NULL, ""}, //isfile
	{NULL, ""}, //fileparts
	{NULL, ""}, //movefile
	{NULL, ""}, //basename
	{NULL, ""}, //pathconvert
	{NULL, ""}, //cd
    {NULL, ""}, //mgetl
    {NULL, ""}, //mputl
    {NULL, ""}, //tempname
    {NULL, ""}, //getURL
    {NULL, ""}  //splitURL
};
/*--------------------------------------------------------------------------*/
int gw_fileio(void)
{
    return 0;
}
/*--------------------------------------------------------------------------*/

