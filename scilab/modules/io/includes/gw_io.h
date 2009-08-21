/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef __GW_IO_H__
#define __GW_IO_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/
int gw_io(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_read)(char *fname,unsigned long fname_len);
int C2F(sci_oldsave)(char *fname,unsigned long fname_len);
int C2F(sci_write)(char *fname,unsigned long fname_len);
int C2F(sci_rat)(char *fname,unsigned long fname_len);
int C2F(sci_file)(char *fname,unsigned long fname_len);
int C2F(sci_host)(char *fname,unsigned long fname_len);
int C2F(sci_unix)(char *fname,unsigned long fname_len);
int C2F(sci_readb)(char *fname,unsigned long fname_len);
int C2F(sci_writb)(char *fname,unsigned long fname_len);
int C2F(sci_getpid)(char *fname,unsigned long fname_len);
int C2F(sci_getenv)(char *fname,unsigned long fname_len);
int C2F(sci_read4b)(char *fname,unsigned long fname_len);
int C2F(sci_write4b)(char *fname,unsigned long fname_len);
int C2F(sci_save)(char *fname,unsigned long fname_len);
int C2F(sci_load)(char *fname,unsigned long fname_len);
int C2F(sci_mgetl)(char *fname,unsigned long fname_len);
int C2F(sci_getio)(char *fname,unsigned long fname_len);
int C2F(sci_setenv)(char *fname,unsigned long fname_len);

int C2F(intgetenv)(char *fname,unsigned long fname_len);
int C2F(intload) (int *id1, int *k1); /* FORTRAN subroutine */
int C2F(intsave)(void);
int C2F(intfile)(void);
int C2F(intmgetl)(void);
int C2F(intrat)(void);
int C2F(intread)(void);
int C2F(intread4b)(void);
int C2F(intreadb)(void);
int C2F(intwrite)(void);
int C2F(intwrite4b)(void);
int C2F(intwritb)(void);

/*--------------------------------------------------------------------------*/
#endif /* __GW_IO_H__ */
/*--------------------------------------------------------------------------*/

