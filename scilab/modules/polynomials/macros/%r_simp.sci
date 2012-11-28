// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

function r=%r_simp(h)
    if simp_mode() == %f then
        r=h;
        return;
    end

    if typeof(h) <> "rational"
        error(msprintf(gettext("%s : Wrong type for input arguments #%d: A rational expected.\n"), "simp", 1));
    end

    r=h;
    if isreal(h) == %f then
        // implement complex case
        return;
    end

    [r(2), r(3)] = simp(r(2), r(3));
endfunction
