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
/*--------------------------------------------------------------------------*/
#ifndef __STATUS2MODE_H__
#define __STATUS2MODE_H__

/**
* convert status to an int (open a file)
* @param status path example : "rt" "rb"
* @return an integer
*/
int Status2Mode(char *status);

#endif /* __STATUS2MODE_H__ */
/*--------------------------------------------------------------------------*/
