/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Bruno JOFRET
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

#ifndef __SYNCEXEC_H__
#define __SYNCEXEC_H__
/*--------------------------------------------------------------------------*/

/**
* execute a macro in scilab
* @param macro to execute (a string , "a=1+3;")
* @param length of macro (here 6)
* @param code error returned
* @param mode sequential or not
* @param length of macro (again, fortran)
*/
int syncexec(char *str, int *ns, int *ierr, int *seq, long int str_len);

#endif /* __SYNCEXEC_H__ */
/*--------------------------------------------------------------------------*/
