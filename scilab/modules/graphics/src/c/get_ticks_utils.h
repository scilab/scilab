/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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

/*------------------------------------------------------------------------*/
/* file: get_ticks_utils.h                                                */
/* desc : a set of functions used to return ticks tlist in scilab         */
/*------------------------------------------------------------------------*/

#ifndef _GET_TICKS_UTILS_H_
#define _GET_TICKS_UTILS_H_

void* buildTListForTicks(const double* locations, char * labels[], int nbTics);

#endif /* _GET_TICKS_UTILS_H_ */
