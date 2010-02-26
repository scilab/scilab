// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_getallfv --
//   Returns all the function values of all the vertices,
//   as a column vector.
//   The function value of vertex #k is stored in fv(k)
//   with k = 1 , nbve
// Arguments
//   <no arg>
//
function fv = optimsimplex_getallfv ( this )
  fv = this.fv ( 1:this.nbve , 1 )
endfunction

