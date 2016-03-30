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

function R=%st_t(M)

    D=double(M.dims)
    if size(D,"*")>2 then
        error(msprintf(_("%s: Wrong size for input argument #%d: Transpose can not be defined.\n"),"%st_t",1));
    end
    NewInd=matrix(matrix(1:prod(D),D)',1,-1)
    F=getfield(1,M)
    R=mlist(F,int32([D(2) D(1)]))
    for k=3:size(F,"*")
        v=getfield(k,M)
        R=setfield(k,list(v(NewInd)),R)
    end
endfunction
