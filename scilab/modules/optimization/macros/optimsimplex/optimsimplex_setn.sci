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
// optimsimplex_setn --
//   Set the dimension of the space of the simplex.
// Arguments
//   n : the dimension
//
function this = optimsimplex_setn ( this , n )
    if typeof(this) <> "TSIMPLEX" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setn", 1));
    end
    if type(n) <> 1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "optimsimplex_setn", 2));
    end
    if or(size(n) <> [1 1]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optimsimplex_setn", 2));
    end
    if n-floor(n) <> 0 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer expected.\n"), "optimsimplex_setn", 2));
    end
    this.n = n;
endfunction
