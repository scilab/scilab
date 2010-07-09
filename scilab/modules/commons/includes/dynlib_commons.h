/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/ 
#ifndef __DYNLIB_COMMONS_H__
#define __DYNLIB_COMMONS_H__

#ifdef _MSC_VER
	#ifdef COMMONS_EXPORTS
		#define COMMONS_IMPEXP __declspec(dllexport)
	#else
		#define COMMONS_IMPEXP __declspec(dllimport)
	#endif
#else
	#define COMMONS_IMPEXP
#endif

#endif /* __DYNLIB_COMMONS_H__ */
/*--------------------------------------------------------------------------*/ 
