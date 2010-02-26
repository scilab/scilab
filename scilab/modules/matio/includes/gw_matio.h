/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Vincent COUVERT 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*-----------------------------------------------------------------------------------*/ 
#include "dynlib_matio.h"
/*-----------------------------------------------------------------------------------*/ 
#ifndef __GW_MATIO_H__
#define __GW_MATIO_H__
/*-----------------------------------------------------------------------------------*/
MATIO_IMPEXP int gw_matio(void);
/*-----------------------------------------------------------------------------------*/
/* interface for the previous function Table */ 
/*-----------------------------------------------------------------------------------*/ 
MATIO_IMPEXP int sci_matfile_open(char *fname,unsigned long fname_len);
MATIO_IMPEXP int sci_matfile_close(char *fname,unsigned long fname_len);
MATIO_IMPEXP int sci_matfile_listvar(char *fname,unsigned long fname_len);
MATIO_IMPEXP int sci_matfile_varreadnext(char *fname,unsigned long fname_len);
MATIO_IMPEXP int sci_matfile_varwrite(char *fname,unsigned long fname_len);
/*-----------------------------------------------------------------------------------*/
#endif /* __GW_MATIO_H__ */
