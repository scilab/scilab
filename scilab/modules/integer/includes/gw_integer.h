
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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
#ifndef __GW_INTEGER__
#define __GW_INTEGER__
/*--------------------------------------------------------------------------*/
#include "dynlib_integer_gw.h"
#include "dynlib_integer.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
INTEGER_IMPEXP int C2F(sciinttype)(char *fname, unsigned long fname_len);
INTEGER_IMPEXP int C2F(scimgeti)(char *fname, unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciimput)(char *fname, unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciitriu)(char *fname, unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciitril)(char *fname, unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciisum)(char *fname, unsigned long fname_len);
//INTEGER_IMPEXP int C2F(sciimax)(char *fname,unsigned long fname_len);
//INTEGER_IMPEXP int C2F(sciimin)(char *fname,unsigned long fname_len);
//INTEGER_IMPEXP int C2F(sciimaxi)(char *fname,unsigned long fname_len);
//INTEGER_IMPEXP int C2F(sciimini)(char *fname,unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciip)(char *fname, unsigned long fname_len);
INTEGER_IMPEXP int C2F(sciiprod)(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /*  __GW_INTEGER__ */
/*--------------------------------------------------------------------------*/

