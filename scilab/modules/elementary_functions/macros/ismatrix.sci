// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Samuel Gougeon
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = ismatrix(A)
    rhs=argn(2);
    if rhs<>1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"ismatrix", 1));
    end
    if type(A) == 15 then
        r = size(A) <> 0;
        return;
    end

    s = size(squeeze(A))
    s = length(s(s>0))
    r = s>0 & s<3;

endfunction
