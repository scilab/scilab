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
function f=%b_e(varargin)
    //A(i,j,k,..)

    rhs=size(varargin)
    M=varargin(rhs)

    nind=rhs-1
    dims=[]
    for k=3:nind
        ind=varargin(k)
        if type(ind)==2|type(ind)==129 then ind=horner(ind,1),end
        if type(ind)==4 then ind=find(ind),end
        if or(ind<>1) then error(21),end
        n=size(ind,"*")
        dims=[dims,n]
    end
    f=M(varargin(1:min(2,rhs-1)))
    k=find(dims>1)
    if k<>[] then
        dims(k($)+1:$)=[]
        N=prod(dims)
        szf=size(f)
        f=f(:)
        f=matrix(f(:,ones(1,N)), [szf dims])
    end
endfunction
