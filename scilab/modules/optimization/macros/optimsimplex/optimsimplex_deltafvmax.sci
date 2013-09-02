// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

//
// optimsimplex_deltafvmax --
//   Returns the difference of function
//   value between the high and the low vertices.
//   It is expected that the vertex #1 is associated with
//   the smallest function value and that the vertex #nbve
//   is associated with the highest function value.
// Arguments
//   <no arg>
// Note
//   Since vertices are ordered, the high
//   is greater than the low.
//
function dfm = optimsimplex_deltafvmax ( this )
    dfm = this.fv(this.nbve) - this.fv(1)
endfunction

