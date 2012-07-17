/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _ISATTY_HH_
#define _ISATTY_HH_

#ifdef _MSC_VER
int isatty (int desc);
#else
#include <unistd.h>
#endif

#endif /* !_ISATTY_HH_ */

