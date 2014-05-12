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

function M=%hm_rand(varargin)

    //creates a random hypermatrix with shape given by vector of dimensions or an
    //hypermatrix
    nv=size(varargin)
    if nv==1 then
        M=varargin(1)
        if type(M)==1 then
            dims=M
        else
            dims=M("dims")
        end
        M=matrix(rand(prod(double(dims)),1), dims)
    else

        if type(varargin($))==10 then
            opt=varargin($),nv=nv-1;
            dims=ones(1,nv)
            for k=1:nv,dims(k)=int(varargin(k)),end
            //remove highest singleton dimensions
            ks=find(dims==1);
            ns=size(ks,"*");
            while ns>0&nv>2&ks(ns)==nv then ns=ns-1,nv=nv-1,end
            if nv==2 then
                M=rand(varargin(1:nv),opt)
            else
                M=matrix(rand(prod(dims(1:nv)),1,opt), dims(1:nv))
            end
        else
            dims=ones(1,nv)
            for k=1:nv,dims(k)=int(varargin(k)),end
            //remove highest singleton dimensions
            ks=find(dims==1);
            ns=size(ks,"*");
            while ns>0&nv>2&ks(ns)==nv then ns=ns-1,nv=nv-1,end
            if nv==2 then
                M=rand(varargin(1:nv))
            else
                M=matrix(rand(prod(dims(1:nv)),1), dims(1:nv))
            end
        end
    end
endfunction
