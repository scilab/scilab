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
// optimsimplex_setfv --
//   Set the function value at given index and
//   returns an updated simplex.
// Arguments
//   ive : vertex index
//   fv : the function value
//
function this = optimsimplex_setfv ( this , ive , fv )
    if typeof(this) <> "TSIMPLEX" then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: TSIMPLEX expected.\n"), "optimsimplex_setfv", 1));
    end
    if type(ive) <> 1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "optimsimplex_setfv", 2));
    end
    if type(fv) <> 1 then
        error(msprintf(gettext("%s: Wrong type for input argument #%d: A real vector expected.\n"), "optimsimplex_setfv", 3));
    end
    if or(size(ive) <> [1 1]) then
        error(msprintf(gettext("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "optimsimplex_setfv", 2));
    end
    if ive-floor(ive) <> 0 then
        error(msprintf(gettext("%s: Wrong value for input argument #%d: An integer expected.\n"), "optimsimplex_setfv", 2));
    end
    this.fv ( ive , 1 ) = fv;
endfunction
