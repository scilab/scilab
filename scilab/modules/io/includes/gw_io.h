/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
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

#ifndef __GW_IO_H__
#define __GW_IO_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "dynlib_io.h"
/*--------------------------------------------------------------------------*/
IO_IMPEXP int gw_io(void);
/*--------------------------------------------------------------------------*/

//Scilab 6
int sci_getenv(char *fname, void* pvApiCtx);
int sci_setenv(char *fname, void* pvApiCtx);

//int sci_read(char *fname, unsigned long fname_len);
int sci_oldsave(char *fname, unsigned long fname_len);
//int sci_write(char *fname, unsigned long fname_len);
int sci_readb(char *fname, unsigned long fname_len);
int sci_writb(char *fname, unsigned long fname_len);
int sci_getpid(char *fname, unsigned long fname_len);
int sci_read4b(char *fname, unsigned long fname_len);
int sci_write4b(char *fname, unsigned long fname_len);
int sci_save(char *fname, void *pvApiCtx);
int sci_percent_load(char *fname, unsigned long fname_len);
int sci_getio(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_IO_H__ */
/*--------------------------------------------------------------------------*/

