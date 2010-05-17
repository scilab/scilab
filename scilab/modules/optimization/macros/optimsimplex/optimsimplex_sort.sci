// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimsimplex_sort --
//   Sorts the simplex with increasing function value order so that 
//   the smallest function value is at vertex #1.
// Arguments
//   <no arg>
//
function this = optimsimplex_sort ( this )
  [this.fv,is] = gsort(this.fv,"r","i");
  this.x = this.x ( is , : );
endfunction

