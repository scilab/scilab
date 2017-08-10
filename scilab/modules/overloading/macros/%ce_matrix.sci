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
