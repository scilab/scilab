// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_spendley --
//   Configure the current simplex so that it is computed from Spendley's method, 
//   i.e. a regular simplex made of k = n+1 vertices.
// Arguments
//   x0 : the initial point, as a row vector
//   fun : name of the function
//   len : the length of the simplex
//   data : user-defined data
//
function [ this , data ] = optimsimplex_spendley ( this , x0 , fun , len , data )
  if (~isdef('len','local')) then
   len = 1.0;
  end
  n = length(x0);
  this.n = n;
  this.nbve = n + 1;
  this.x = zeros ( n , n+1 )
  this.fv = zeros ( n+1 )
  //
  // Compute p (diagonal term) , q (off-diagonal term)
  //
  p  = (n - 1.0 + sqrt(n + 1))/(n * sqrt(2.0))
  q = (sqrt(n + 1) - 1.0)/(n * sqrt(2.0))
  //
  // Set 1st point
  //
  this.x ( 1:n,1 ) = x0 (1:n)'
  //
  // Set points #2 to #n+1
  //
  for j = 2 : this.n+1
    // Note : Vectorize when possible
    // In order to vectorize, add q everywhere, then substract p just for one diagonal term j-1
    this.x ( 1:n,j ) = x0 (1:n)' + len * q * ones(n,1)
    this.x ( j-1,j ) = this.x ( j-1,j ) - len * q + len * p
  end
  // Compute Function Value
  if (~isdef('data','local')) then
    this = optimsimplex_computefv ( this , fun )
  else
    [ this , data ] = optimsimplex_computefv ( this , fun , data )
  end
endfunction

