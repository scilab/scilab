// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//
// optimsimplex_getve --
//   Returns the vertex at given index as a tlist, 
//   with fields n, x and fv
// Arguments
//   ive : vertex index
//
function vertex = optimsimplex_getve ( this , ive )
  vertex = tlist(["T_VERTEX","x","n","fv"]);
  vertex.n = this.n;
  vertex.x = this.x ( ive , : );
  vertex.fv = this.fv ( ive );
endfunction

