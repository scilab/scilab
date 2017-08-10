// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function b=%s_c_cblock(a,b)
    if a==[] then return,end
    v2=getfield(2,b)

    if size(a,1)<>size(v2,1) then error(5),end
    if type(a)==type(v2) then
        b=setfield(2,[a v2],b)
    else
        b=setfield(0,"cblock",b)
        b=setfield(2,a,b);
    end
endfunction
