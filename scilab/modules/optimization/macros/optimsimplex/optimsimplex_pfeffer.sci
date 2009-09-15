// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_pfeffer --
//   Configure the current simplex so that it is computed from Pfeffer's method, 
//   i.e. a relative delta for non-zero values and an absolute delta 
//   for zero values.
// Arguments
//   x0 : the initial point, as a row vector
//   fun : name of the function
//   deltausual : the absolute delta for non-zero values
//   deltazero : the absolute delta for zero values
//   data : user-defined data
// References
//   "Global Optimization Of Lennard-Jones Atomic Clusters"
//   Ellen Fan, Thesis, February 26, 2002, McMaster University
//   Method due to L. Pfeffer at Stanford
//
function [ this , data ] = optimsimplex_pfeffer ( this , x0 , fun , deltausual , deltazero , data )
   if (~isdef('deltausual','local')) then
    deltausual = 0.05
  end
   if (~isdef('deltazero','local')) then
    deltazero = 0.0075
  end
  n = length(x0);
  this.n = n;
  this.nbve = n + 1;
  this.x = zeros ( n , n+1 )
  this.fv = zeros ( n+1 )
  //
  // Set 1st point
  //
  this.x ( 1:n,1 ) = x0 (1:n)'
  //
  // Set points #2 to #n+1
  //
  for j = 2 : this.n+1
    this.x ( 1:n,j ) = x0 (1:n)'
    if x0( j-1 ) == 0.0 then
      this.x ( j-1,j ) = deltazero
    else
      this.x ( j-1,j ) = this.x ( j-1,j ) + deltausual * x0( j-1 )
    end
  end
  // Compute Function Value
  if (~isdef('data','local')) then
    this = optimsimplex_computefv ( this , fun )
  else
    [ this , data ] = optimsimplex_computefv ( this , fun , data )
  end
endfunction

