// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2003 - INRIA - Carlos Klimann
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

function s = samwr(sizam, numsamp, X)
    //
    //This  function gives  a  s matrix  sizsam  x numsamp.   It
    //contains  numsamp  random samples  (the  columns) each  of
    //sizam  ( < size(X, '*'))  extractions,  without  replacement,
    //from the  vector X.
    //
    if argn(2) <> 3 then
        error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"), "samwr", 3)),
    end
    if X == []|sizam == 0|numsamp == 0 then s = []; return; end
    sizx = size(X, "*")
    if sizam > sizx then
        error(msprintf(gettext("%s: Wrong value of input argument #%d: Lower than or equal to size of input argument #%d expected.\n"), "samwr", 1, 3)),
    end

    perm = grand(numsamp, "prm", (1:sizx)')
    s = matrix(X(perm), -1, numsamp)
    s = s(1:sizam, :)
endfunction
