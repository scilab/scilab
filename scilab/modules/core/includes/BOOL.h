/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __BOOL_H__
#define __BOOL_H__

typedef int integer;

/* define boolean type */
#ifdef BOOL
	#undef BOOL
#endif

typedef int BOOL ;

#ifdef TRUE
	#undef TRUE
#endif
#define TRUE 1

#ifdef FALSE
	#undef FALSE
#endif
#define FALSE 0

/* converts BOOL to bool */
#define BOOLtobool(w)     ((w == TRUE) ? true : false)

/* converts bool to BOOL */
#define booltoBOOL(w)     ((w == true) ? TRUE : FALSE)          

#endif /* __BOOL_H__ */
/*--------------------------------------------------------------------------*/
