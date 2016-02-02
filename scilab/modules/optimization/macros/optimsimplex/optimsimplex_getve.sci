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
// optimsimplex_getve --
//   Returns the vertex at given index as a tlist,
//   with fields n, x and fv
// Arguments
//   ive : vertex index
//
function vertex = optimsimplex_getve ( this , ive )
    if typeof(this) <> "TSIMPLEX" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_getve", 1));
    end
    if type(ive) <> 1| or(size(ive) <> [1 1]) then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "optimsimplex_getve", 2));
    end
    if ive-floor(ive) <> 0 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer expected.\n"), "optimsimplex_getve", 2));
    end
    vertex = tlist(["T_VERTEX","x","n","fv"]);
    vertex.n = this.n;
    vertex.x = this.x ( ive , : );
    vertex.fv = this.fv ( ive );
endfunction
