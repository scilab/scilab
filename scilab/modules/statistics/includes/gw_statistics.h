/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __GW_STATISTICS__
#define __GW_STATISTICS__
/*--------------------------------------------------------------------------*/
#include "dynlib_statistics.h"
/*--------------------------------------------------------------------------*/
STATISTICS_IMPEXP int gw_statistics(void);
/*--------------------------------------------------------------------------*/
STATISTICS_IMPEXP int cdfbetI(char* fname, unsigned long l);
STATISTICS_IMPEXP int cdfbinI(char* fname, unsigned long l);
STATISTICS_IMPEXP int cdfchiI(char* fname, unsigned long l);
STATISTICS_IMPEXP int cdfchnI(char* fname, unsigned long l);
STATISTICS_IMPEXP int cdffI(char* fname, unsigned long l);
STATISTICS_IMPEXP int cdffncI(char* fname, unsigned long l);
STATISTICS_IMPEXP int cdfgamI(char* fname, unsigned long l);
STATISTICS_IMPEXP int cdfnbnI(char* fname, unsigned long l);
STATISTICS_IMPEXP int cdfnorI(char* fname, unsigned long l);
STATISTICS_IMPEXP int cdfpoiI(char* fname, unsigned long l);
STATISTICS_IMPEXP int cdftI(char* fname, unsigned long l);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_STATISTICS__ */
/*--------------------------------------------------------------------------*/

