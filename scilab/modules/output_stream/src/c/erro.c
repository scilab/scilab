/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "erro.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(erro)(char *str,unsigned int str_len)
{
  Scierror(9999,str);
  return 0;
}
/*--------------------------------------------------------------------------*/
