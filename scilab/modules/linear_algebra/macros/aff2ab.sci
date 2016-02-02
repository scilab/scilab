
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [A,b]=aff2ab(lme,dimX,D,flag)
    //Y,X,D are lists of matrices.
    //Y=lme(X,D)= affine fct of Xi's;
    //[A,b]=matrix representation of lme in canonical basis.
    // if flag=='sp' A matrix is return in sparse storage.
    [LHS,RHS]=argn(0)
    if RHS==3 then flag="f";end

    nvars=0;
    for k=dimX'
        nvars=nvars+prod(k);
    end
    if part(flag,1)=="f" then
        x0=zeros(nvars,1);
        b=list2vec(lme(vec2list(x0,dimX),D));
        [p,un]=size(b);
        A=zeros(p,nvars);
        for k=1:nvars
            xi=x0;xi(k)=1;
            A(:,k)=list2vec(lme(vec2list(xi,dimX),D))-b;
            //   A=[A,sparse(list2vec(lme(vec2list(xi,dimX),D))-b)];
        end
    end

    if part(flag,1)=="s" then
        x0=zeros(nvars,1);
        b=list2vec(lme(vec2list(x0,dimX),D));
        A=[];
        for k=1:nvars
            xi=x0;xi(k)=1;
            A=[A,sparse(list2vec(lme(vec2list(xi,dimX),D))-b)];
        end
    end
endfunction
