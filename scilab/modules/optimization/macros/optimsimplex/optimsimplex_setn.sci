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
