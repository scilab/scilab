/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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

