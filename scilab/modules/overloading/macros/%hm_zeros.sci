// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function M=%hm_zeros(varargin)

    //creates a zero hypermatrix with shape given by vector of dimensions or an
    //hypermatrix
    nv=size(varargin)
    if nv==1 then
        M=varargin(1)
        if type(M)==1 then
            dims=M
        else
            dims=M("dims")
        end
        M=hypermat(dims,zeros(prod(double(dims)),1))
    else
        dims=ones(1,nv)
        for k=1:nv,dims(k)=int(varargin(k)),end
        //remove highest singleton dimensions
        ks=find(dims==1);
        ns=size(ks,"*");
        while ns>0&nv>2&ks(ns)==nv then ns=ns-1,nv=nv-1,end
        if nv==2 then
            M=zeros(varargin(1:nv))
        else
            M=hypermat(dims(1:nv),zeros(prod(dims(1:nv)),1))
        end

    end
endfunction
