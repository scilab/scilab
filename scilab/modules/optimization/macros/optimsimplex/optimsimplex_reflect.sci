// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_reflect --
//   Returns a new simplex by reflexion of current simplex, by reflection with respect 
//   to the first vertex in the simplex.
//   This move is used in the centered simplex gradient.
// Arguments
//   fun : name of the function
//   data : user-defined data
//
function [ r , data ] = optimsimplex_reflect ( this , fun , data )
  r = optimsimplex_new ( )
  r.n = this.n
  r.nbve = this.nbve
  r.x = zeros ( r.nbve , r.n )
  r.fv = zeros( r.nbve , 1 )
  r.x(1,1:r.n) = this.x(1,1:r.n)
  r.fv(1) = this.fv(1)
  x1 = this.x(1,1:r.n)
  twox1 = 2*x1
  for i = 2:r.nbve
    r.x(i,1:r.n) = twox1 - this.x(i,1:r.n)
    if (~isdef('data','local')) then
       r.fv(i)  = fun (r.x(i,:));
     else
       [ r.fv(i) , data ]  = fun (r.x(i,:) , data );
     end
  end
endfunction

