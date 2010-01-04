// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_getall --
//   Returns all the coordinates of all the vertices and the function values
//   in the same matrix, with size nbve X n+1, which is organized as follows :
//   * data is organized by row : function value, then x
//   * simplex(k,1) is the function value of the vertex #k, with kve = 1 , nbve
//   * simplex(k,2:n+1) is the coordinates of the vertex #k, with kve = 1 , nbve
// Arguments
//   <no arg>
//
function simplex = optimsimplex_getall ( this )
  simplex = zeros ( this.nbve , this.n+1 );
  simplex ( 1:this.nbve , 1 ) = this.fv ( 1:this.nbve , 1 )
  simplex ( 1:this.nbve , 2:this.n+1 ) = this.x ( 1:this.nbve , 1:this.n )
endfunction

