/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "banier.h"
#include "csignal.h"
#include "sigbas.h"

extern int C2F(sigbas)();

void controlC_handler (int sig)
{
  int j = SIGINT;
  C2F(sigbas)(&j);
}

int C2F(csignal)(void)
{
  signal (SIGINT, controlC_handler);
  return(0);
}

