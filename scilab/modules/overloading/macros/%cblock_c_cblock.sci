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
function a=%cblock_c_cblock(a,b)
    na=length(a)
    if na==1 then a=b,return,end
    if length(b)==1 then return,end
    v1=getfield(na,a)
    v2=getfield(2,b)

    if size(v1,1)<>size(v2,1) then error(5),end
    if type(v1)==type(v2) then
        a=setfield(na,[v1 v2],a)
    else
        a=setfield(na+1,v2,a);na=na+1
    end

    for k=3:length(b)
        a=setfield(na+1,getfield(k,b),a)
        na=na+1
    end
endfunction
