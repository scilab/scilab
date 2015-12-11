// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
        setfield(k,list(v(NewInd)),R)
    end
endfunction
