// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_oriented --
//   Returns an oriented simplex, in sorted order. 
//   This simplex may be used, as Kelley suggests
//   for a restart of Nelder-Mead algorithm.
// Arguments
//   fun : the function to compute at vertices
//   data : user-defined data
//
function [ ns , data ] = optimsimplex_oriented ( this , fun , data )
  if ( this.nbve <> this.n+1 ) then
    errmsg = msprintf(gettext ( "%s: The oriented simplex can be computed only with a simplex made of n+1 points, but the dimension is %d and the number of vertices is %d") , "optimsimplex_oriented", this.n , this.nbve)
    error(errmsg)
  end
  sgrad = optimsimplex_gradientfv ( this )
  ssize = optimsimplex_size ( this , "sigmaminus" )
  n = this.n
  // Compute the betas
  betav = zeros(n,1)
  for i = 1:n
    if sgrad(i)==0.0 then
      betav(i)  = ssize
    elseif sgrad(i) > 0.0 then
      betav(i)  = ssize
    else
      betav(i)  = -ssize
    end
  end
  betav = -0.5 * betav
  // Prepare a matrix with beta as diagonal terms
  mid = eye ( n , n )
  for i = 1:n
    mid (i,i) = betav(i)
  end
  // Compute simplex
  ns = optimsimplex_new()
  ns.n = this.n
  ns.nbve = this.n+1
  ns.x = zeros ( n+1 , n )
  ns.fv = zeros ( n+1 , 1 )
  // Store 1st point
  ns.x ( 1 , 1:n ) = this.x ( 1 , 1:n )
  ns.fv ( 1 ) = this.fv ( 1 )
  x1 = this.x ( 1 , 1:n )
  for i = 2:n+1
    ns.x ( i, 1:n ) = mid ( i-1 , 1:n ) + x1 ( 1 , 1:n )
    if (~isdef('data','local')) then
       ns.fv(i)  = fun (ns.x(i,:));
     else
       [ ns.fv(i) , data ]  = fun (ns.x(i,:) , data );
     end
  end
endfunction

