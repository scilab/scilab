/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include "x_main.h"

#if defined(linux) && defined(__i386__)
#include "setPrecisionFPU.h"
#endif
/*--------------------------------------------------------------------------*/
int main(int argc, char **argv)
{
  /* This bug only occurs under Linux 32 bits */
#if defined(linux) && defined(__i386__)
  setFPUToDouble();
#endif
   /* call the main of Scilab */
   mainscic(argc,argv);
   return 0;
}
/*--------------------------------------------------------------------------*/
