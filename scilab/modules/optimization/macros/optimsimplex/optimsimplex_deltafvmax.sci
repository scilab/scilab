// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008-2009 - INRIA - Michael Baudin
// Copyright (C) 2009-2010 - DIGITEO - Michael Baudin
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

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

