// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_setve --
//   Sets the coordinates of the vertex and the function value at given index in the current simplex.
// Arguments
//   ive : vertex index
//   fv : the function value
//   x : the coordinates of the point, as a row vector
//
function this = optimsimplex_setve ( this , ive , fv , x )
  this.x(ive,:) = x;
  this.fv(ive,1) = fv;
endfunction

