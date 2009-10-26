// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_computefv --
//   Set the values of the function at vertices points.
// Arguments
//   fun : the function to compute at vertices
//   data : user-defined data
//
function [ this , data ] = optimsimplex_computefv ( this , fun , data )
  if (~isdef('data','local')) then
    this = optimsimplex_compsomefv ( this , fun , 1 : this.nbve )
  else
    [ this , data ] = optimsimplex_compsomefv ( this , fun , 1 : this.nbve , data )
  end
endfunction

