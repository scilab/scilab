/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) Scilab Enterprises - 2013 - Antoine ELIAS
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#ifndef __DYNLIB_MPI_H__
#define __DYNLIB_MPI_H__

#ifdef _MSC_VER
#ifdef MPI_EXPORTS
#define MPI_IMPEXP __declspec(dllexport)
#else
#define MPI_IMPEXP __declspec(dllimport)
#endif
#else
#define MPI_IMPEXP
#endif

#endif /* __DYNLIB_MPI_H__ */
/*--------------------------------------------------------------------------*/
