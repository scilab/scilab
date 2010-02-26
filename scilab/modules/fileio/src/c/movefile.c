/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "movefile.h"
#include "deleteafile.h"
#include "removedir.h"
#include "copyfile.h"
#include "createdirectory.h"
#include "isdir.h"
/*--------------------------------------------------------------------------*/
int MoveFileFunction(wchar_t *DestinationFilename, wchar_t *SourceFilename)
{
	int ierr = 0;
	ierr = CopyFileFunction(DestinationFilename, SourceFilename);
	if (ierr == 0)
	{
		if (deleteafileW(SourceFilename)) return 0;
		else return 1;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int MoveDirectoryFunction(wchar_t *DestinationDirectory, wchar_t *SourceDirectory)
{
	int ierr = 0;

	if ( !isdirW(DestinationDirectory)  )
	{
		createdirectoryW(DestinationDirectory);
	}

	ierr = CopyDirectoryFunction(DestinationDirectory, SourceDirectory);
	if (ierr == 0)
	{
		if (removedirW(SourceDirectory)) return 0;
		else return 1;
	}
	return 0;

}
/*--------------------------------------------------------------------------*/
