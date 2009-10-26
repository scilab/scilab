// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_setallfv --
//   Set all the function values of all the vertices.
//   The vertex #k is expected to be stored in fv(k)
//   with k = 1 , nbve
// Arguments
//   fv : the array of function values
//
function this = optimsimplex_setallfv ( this , fv )
  fv1 = size ( fv , 1 );
  if fv1 <> this.nbve then
    error ( msprintf ( gettext ( "%s: The number of rows in the function value array is %d, while expected %d." ), "optimsimplex_setallfv" , fv1 , this.nbve ))
  end
  fv2 = size ( fv , 2 );
  if fv2 <> 1 then
    error ( msprintf ( gettext ( "%s: The number of columns in the function value array is %d, while expected 1." ), "optimsimplex_setallfv" , fv2 ))
  end
  this.fv ( 1:this.nbve , 1 ) = fv ( 1:this.nbve );
endfunction

