/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#ifndef __BACKTRACE_PRINT_H__
#define __BACKTRACE_PRINT_H__

/*
 * Build a backtrace.
 * @param first_level stack level where to begin to print
 * @param unmangle if !=0 then demangle the functions names
 * @return a string (to be freed) containing the complete backtrace
 */
char * backtrace_print(int first_level, int unmangle);

#endif // __BACKTRACE_PRINT_H__
