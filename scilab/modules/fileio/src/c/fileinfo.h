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

#ifndef __FILEINFO_H__
#define __FILEINFO_H__

#define FILEINFO_ARRAY_SIZE 13

double * fileinfo(char *filename, int *ierr);

double * filesinfo(char **filenames, int dim_filenames, int *ierrs);

#endif /* __FILEINFO_H__ */
/*--------------------------------------------------------------------------*/
