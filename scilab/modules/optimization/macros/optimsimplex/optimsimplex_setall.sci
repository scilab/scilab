// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_setall --
//   Set all the coordinates and and the function values of all the vertices.
//   The given matrix is expected to have the following organization
//   * size nbve X n+1
//   * data is organized by row : function value, then x
//   * simplex(k,1) is the function value of the vertex #k, with k = 1 , nbve
//   * simplex(k,2:n+1) is the coordinates of the vertex #k, with k = 1 , nbve
// Arguments
//   simplex : the simplex to set
//
function this = optimsimplex_setall ( this , simplex )
  nbve = size(simplex,1)
  np1 = size(simplex,2)
  if np1 > nbve then
    errmsg = msprintf(gettext ( "%s: The number of vertices (i.e. the number of rows) is %d which is smaller than the number of columns %d (i.e. n+1).") , "optimsimplex_setall",nbve,np1);
    error(errmsg);
  end
  this.n = np1 - 1;
  this.nbve = nbve;
  this.fv ( 1:nbve , 1 ) = simplex ( 1:nbve , 1 )
  this.x ( 1:nbve , 1:this.n ) = simplex ( 1:nbve , 2:this.n+1 )
endfunction

