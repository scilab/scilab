// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2018 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function b = %generic_c_cblock(a, b)
    if a==[] then return,end
    emptyTable = prod(size(b))==0
    if ~emptyTable
        v = getfield(2, b)
        if size(v,1) <> size(a,1) then
            msg = _("%s: Arguments #%d and #%d: Incompatible sizes.\n")
            error(msprintf(msg, CallingOverloadName, 1, 2))
        end
        if type(a)==type(v) then
            b = setfield(2, [a v], b)
        else
            b = setfield(0, "cblock",b)
            b = setfield(2, a, b);
        end
    else
        b = setfield(2, a, b)
    end
endfunction
