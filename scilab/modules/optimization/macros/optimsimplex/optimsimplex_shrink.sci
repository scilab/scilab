// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_shrink --
//   Shrink the simplex with given coefficient sigma
//   and returns an updated simplex.
//   The shrink is performed with respect to the 1st point
//   in the simplex.
// Arguments
//   fun : the function to compute at vertices
//   sigma : shrinkage factor (default value = 0.5)
//   data : user-defined data
//
function [ this , data ] = optimsimplex_shrink ( this , fun , sigma , data )
  if (~isdef('sigma','local')) then
    sigma = 0.5;
  end
  nv = this.nbve;
  mv1 = this.x(1,:) .*. ones ( nv - 1 , 1 );
  newx = ( 1.0 - sigma ) * mv1(1:nv-1,:) + sigma * this.x ( 2 : nv , : ) ;
  this.x(2:nv,:) = newx(1:nv-1,:);
  if (~isdef('data','local')) then
    this = optimsimplex_compsomefv ( this , fun , 2 : nv )
  else
    [ this , data ] = optimsimplex_compsomefv ( this , fun , 2 : nv , data )
  end
endfunction

