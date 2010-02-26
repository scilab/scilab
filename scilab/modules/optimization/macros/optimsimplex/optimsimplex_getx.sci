// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_getx --
//   Returns the coordinates of the vertex at given index,
//   as a row vector.
// Arguments
//   ive : vertex index
//
function x = optimsimplex_getx ( this , ive )
  x = this.x ( ive , : )
endfunction

