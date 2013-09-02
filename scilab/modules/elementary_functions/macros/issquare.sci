// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Scilab Enterprises - Samuel Gougeon
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r = issquare(v)
    rhs=argn(2);
    if rhs<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"issquare", 1));
    end
    if type(v) == 15 then
        r = size(v) == 1;
        return;
    end

    s = size(squeeze(v))
    s = s(s>0)   // to reject [ ]
    r = length(unique(s))==1;

endfunction
