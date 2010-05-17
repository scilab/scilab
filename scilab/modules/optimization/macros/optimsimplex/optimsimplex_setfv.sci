// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_setfv --
//   Set the function value at given index and 
//   returns an updated simplex.
// Arguments
//   ive : vertex index
//   fv : the function value
//
function this = optimsimplex_setfv ( this , ive , fv )
  this.fv ( ive , 1 ) = fv;
endfunction

