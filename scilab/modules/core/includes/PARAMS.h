/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __PARAMS_H__
#define __PARAMS_H__

#if defined(__STDC__) || defined(_MSC_VER)
	#ifndef  __PARAMS
		#define  __PARAMS(paramlist)		paramlist
	#endif

	#ifndef  _PARAMS
		#define  _PARAMS(paramlist)		paramlist
	#endif
#else
	#ifndef  __PARAMS
		#define  __PARAMS(paramlist)		()
	#endif
	
	#ifndef  _PARAMS
		#define  _PARAMS(paramlist)		()
	#endif
#endif

#endif /* __PARAMS_H__ */

