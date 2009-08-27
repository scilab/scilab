// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_axes --
//   Configure the current simplex so that it is computed from the axes and a length.
// Arguments
//   x0 : the initial point, as a row vector
//   fun : name of the function
//   length : the length of the simplex
//     If length is a value, that unique length
//     is used in all directions.
//     If length is a vector with n values, each 
//     length is used with the corresponding 
//     direction.
//   data : user-defined data
//
function [ this , data ] = optimsimplex_axes ( this , x0 , fun , len , data )
  if (~isdef('len','local')) then
    len = 1.0;
  end
  n = length(x0);
  this.n = n;
  this.nbve = n + 1;
  nl=length(len)
  if nl==1 then
    xlen = len * ones(n,1)
  else
    xlen = len
  end
  this.x = zeros ( n , this.nbve )
  this.fv = zeros ( this.nbve )
  //
  // Set 1st point
  //
  this.x ( 1:n,1 ) = x0 (1:n)'
  //
  // Set points #2 to #n+1
  //
  for j = 2 : this.nbve
    this.x ( 1:n,j ) = x0 (1:n)'
    this.x ( j-1,j ) = this.x ( j-1,j ) + xlen(j-1)
  end
  // Compute Function Value
  if (~isdef('data','local')) then
    this = optimsimplex_computefv ( this , fun )
  else
    [ this , data ] = optimsimplex_computefv ( this , fun , data )
  end
endfunction

