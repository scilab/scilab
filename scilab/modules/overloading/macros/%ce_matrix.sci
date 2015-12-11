// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
function m=%ce_matrix(m,varargin)
    dims=m.dims
    new=[]
    for v=varargin
        new=[new,v]
    end
    n=find(new==-1)
    if size(n,"*")==1 then
        d=prod(dims)/abs(prod(new))
        if d<>int(d) then error(60),end
        new(n)=d
    elseif n<>[] then
        error(42)
    end
    if prod(new)<>prod(dims) then error(60),end
    m.dims=new
endfunction
