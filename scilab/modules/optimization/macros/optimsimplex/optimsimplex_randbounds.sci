// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


//
// optimsimplex_randbounds --
//   Configure the current simplex so that it is computed by taking the bounds
//   into account with random scaling.
// Arguments
//   x0 : the initial point
//   nbpts : the number of vertices in the simplex (+ the initial point)
//   fun : name of the function
//   boundsmin : array of minimum bounds
//   boundsmax : array of maximum bounds
//   nbve : total number of vertices in the simplex
//   data : user-defined data
//
function [ this , data ] = optimsimplex_randbounds ( this , x0 , fun , boundsmin , boundsmax , nbve  , data )
    n = length ( x0 )
    this.n = n;
    this.nbve = nbve;
    this.x = zeros ( n , nbve )
    this.fv = zeros ( nbve )
    //
    // Set 1st point
    //
    this.x ( 1:n,1 ) = x0 (1:n)'
    //
    // Set points #2 to #nbve, by randomizing the bounds
    //
    for jve = 2 : nbve 
      //
      // Compute vertex coordinates, as a random number 
      // between min and max bounds.
      //
      for ix  = 1 : n
        this.x ( ix , jve ) = boundsmin( ix ) + rand() * (boundsmax( ix ) - boundsmin( ix ))
      end
    end
  // Compute Function Value
  if (~isdef('data','local')) then
    this = optimsimplex_computefv ( this , fun )
  else
    [ this , data ] = optimsimplex_computefv ( this , fun , data )
  end
endfunction

