
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef  __GW_INTERPOLATION_H__
#define __GW_INTERPOLATION_H__
/*--------------------------------------------------------------------------*/
#include "machine.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/
int gw_interpolation(void);
/*--------------------------------------------------------------------------*/
int intsplin(char *fname,unsigned long fname_len); 
int intlsq_splin(char *fname,unsigned long fname_len);
int intinterp1(char *fname,unsigned long fname_len); 
int intlinear_interpn(char *fname,unsigned long fname_len);
int intsplin2d(char *fname,unsigned long fname_len); 
int intinterp2d(char *fname,unsigned long fname_len); 
int intcshep2d(char *fname,unsigned long fname_len); 
int inteval_cshep2d(char *fname,unsigned long fname_len);
int intsplin3d(char *fname,unsigned long fname_len); 
int intinterp3d(char *fname,unsigned long fname_len); 
int intbsplin3val(char *fname,unsigned long fname_len) ;
/*--------------------------------------------------------------------------*/
#endif /* __GW_INTERPOLATION_H__ */
/*--------------------------------------------------------------------------*/

