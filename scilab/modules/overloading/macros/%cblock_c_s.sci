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
function a=%cblock_c_s(a,b)
    if b==[] then return,end
    v=getfield($,a)

    if size(v,1)<>size(b,1) then error(5),end
    if type(b)==type(v) then
        a=setfield($,[v b],a)
    else
        a=setfield($+1,b,a)
    end
endfunction
