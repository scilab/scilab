// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_dirmat --
//   Returns the n x n matrix of simplex directions i.e. 
//   the matrix of differences of vertices coordinates 
//   with respect to the vertex #1.
// Arguments
//   <no arg>
//
function m = optimsimplex_dirmat ( this )
  m = zeros(this.n,this.n)
  for i=1:this.nbve-1
    m(1:this.n,i) = (this.x(i+1,1:this.n) - this.x(1,1:this.n)).'
  end
endfunction

