/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 1998-2000 - ENPC - Jean-Philippe Chancelier
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "cs2st.h"

int C2F(cs2st)(int *i,int *ires)
{
  unsigned int ui;
  ui = (unsigned int) *i;
  ui = (ui/4)+1;
  *ires = (ui/2)+1;
  return 0;
}
