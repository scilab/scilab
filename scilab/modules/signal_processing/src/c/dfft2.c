/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Allan SIMON
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 
#include "gw_signal.h"

void dfft2 ( double* a , double* b , int nseg , int n , int nspn , int isn , int ierr, int* iw , int lw )
{
   iw[0] = 0 ;
   iw[1] = 10 ;
   iw[2] = 10 ;
   iw[3] = lw ;
   iw[4] = 10 ;

   dfftbi ( a , b , nseg , n , nspn , isn , ierr , iw[0], iw[1], iw[2], iw[3], iw[4], iw, iw);

   return ;
}