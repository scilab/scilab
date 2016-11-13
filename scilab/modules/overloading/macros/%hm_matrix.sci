// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function a = %hm_matrix(a, varargin)
    [lhs, rhs] = argn(0)
    if rhs == 0
        a = [];
        varargin = [];
    end

    if size(varargin)==0 then
        if or(floor(a)<>a)|or(a<0) then
            error(msprintf(_("%s: Wrong values for input argument #%d: Elements must be non-negative integers.\n"), "%hm_matrix", 1));
        end
        if  a==[]|or(a==0) then a=[0 0], end

        if size(a, "*")==1 then
            a=[1 a],
        else
            //remove last dimensions equal to 1
            nd=size(a, "*")
            while nd>2&a(nd)==1 then nd=nd-1, end
            a=a(1:nd)
        end
        if size(a, "*")==2 then
            a=zeros(a(1), a(2))
            return
        else
            v=zeros(prod(a), 1)
            a=mlist(["hm", "dims", "entries"], int32(matrix(a, 1, -1)), matrix(v, -1, 1))
            return
        end
    end

    dims=size(a); nd=size(dims, "*")
    if size(varargin)==1 then
        sz=varargin(1)
    else
        sz=[]
        for k=1:size(varargin)
            if size(varargin(k), "*")<>1|type(varargin(k))<>1 then
                error(msprintf(_("%s: Wrong size for input argument(s): integer scalars expected.\n"), "%hm_matrix"));
            end
            sz=[sz varargin(k)]
        end
        sz=round(sz)
        //remove last dimensions equal to 1
        sz_last=size(sz, "*")
        while sz_last>2&sz(sz_last)==1 then sz_last=sz_last-1, end
        sz=sz(1:sz_last)
    end
    nd1=size(sz, "*")
    if nd1<=0 then error(60), end
    if nd1<=2
        // after removing extra dimensions equal to 1
        // returns the 2D matrix
        // a can also be another type of matrix
        a = matrix(a, sz)
        return
    end
    if nd<nd1 then
        dims(nd+1:nd1)=1
    elseif nd>nd1 then
        dims=[dims(1:nd1-1) prod(dims(nd1:nd))]
    end
    nd=size(dims, "*")
    k=find(sz==-1)
    P=prod(dims)
    if k<>[] then
        if size(k, "*")>1 then
            error(msprintf(_("%s: Only one -1 value admitted.\n"), "%hm_matrix"));
        end
        sz(k)=floor(P/(prod(sz(sz>0))))
    end

    if prod(P)<>prod(sz) then
        error(msprintf(_("%s: Input and output matrices  must have the same number of elements"), "%hm_matrix"));
    end

    if type(a)<>17 then
        a = matrix(a, sz)
        return
    end

endfunction
