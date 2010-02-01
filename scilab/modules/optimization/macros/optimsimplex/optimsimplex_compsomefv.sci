// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_compsomefv --
//   Set the values of the function at given vertices.
// Arguments
//   fun : the function to compute at vertices
//   indices : the indices of the vertices to compute
//   data : user-defined data
//
function [ this , data ] = optimsimplex_compsomefv ( this , fun , indices , data )
  for j = indices;
    if (~isdef('data','local')) then
      this.fv(j)  = fun (this.x(j,:));
    else
      [ this.fv(j) , data ]  = fun (this.x(j,:) , data );
    end
  end
endfunction

