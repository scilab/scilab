/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#ifndef __GW_OUTPUT_STREAM_H__
#define __GW_OUTPUT_STREAM_H__

#include "api_common.h"

/*--------------------------------------------------------------------------*/
int gw_output_stream(void);
/*--------------------------------------------------------------------------*/
int sci_print(char *fname,unsigned long fname_len);
int sci_mprintf(char *fname,unsigned long fname_len);
int sci_msprintf(char *fname,unsigned long fname_len);
int sci_disp(char *fname,unsigned long fname_len);
int sci_diary(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
#endif /* __GW_OUTPUT_STREAM_H__ */
/*--------------------------------------------------------------------------*/

