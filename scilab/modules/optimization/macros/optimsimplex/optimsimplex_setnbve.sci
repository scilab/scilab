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
// optimsimplex_setnbve --
//   Set the number of vertices of the simplex.
// Arguments
//   nbve : the number of vertices
//
function this = optimsimplex_setnbve ( this , nbve )
    if typeof(this) <> "TSIMPLEX" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setnbve", 1));
    end
    if type(nbve) <> 1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "optimsimplex_setnbve", 2));
    end
    if or(size(nbve) <> [1 1]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optimsimplex_setnbve", 2));
    end
    if nbve-floor(nbve) <> 0 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer expected.\n"), "optimsimplex_setnbve", 2));
    end
    this.nbve = nbve;
endfunction
