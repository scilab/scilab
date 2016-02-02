
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA
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

#ifndef __XLS_H__
#define __XLS_H__

/**
* xls_read
*
*/
void xls_read(int *fd, int *cur_pos, double **data, int **chainesind, int *N, int *M, int *err);

/**
* xls_open
*
*/
void xls_open(int *err, int *fd, char ***sst, int *ns, char ***Sheetnames, int** Abspos, int *nsheets);

#endif /* __XLS_H__ */
/*--------------------------------------------------------------------------*/
