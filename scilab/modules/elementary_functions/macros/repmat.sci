// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) - 2011 -  INRIA, Serge Steer
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.
function B = repmat(A,varargin)
    //ajouter test sur les type des elements de varargin
    rhs = argn(2);
    if rhs < 2 then
        error(msprintf(_("%s: Wrong number of input arguments: at least %d expected.\n"), "repmat", 2))
    end

    narg=size(varargin);

    // Test varargin
    if narg==1 then
        // Scalar of vector needed
        if typeof(varargin(1)) <> "constant" then
            error(msprintf(_("%s: Wrong type for input argument #%d: A real scalar or vector expected.\n"), "repmat", 2))
        end
        if size(varargin(1),"*")<>1 & isempty(find(size(varargin(1))==1)) then
            error(msprintf(_("%s: Wrong size for input argument #%d: A real scalar or vector expected.\n"), "repmat", 2))
        end
    else
        for i=1:narg
            if typeof(varargin(i)) <> "constant" then
                error(msprintf(_("%s: Wrong type for input argument #%d: A real scalar expected.\n"), "repmat", i+1))
            end
            if size(varargin(i),"*")<>1 then
                error(msprintf(_("%s: Wrong size for input argument #%d: A real scalar expected.\n"), "repmat", i+1))
            end
        end
    end

    if type(A)>10 then
        if typeof(A)=="rational" then
            B=rlist(repmat(A.num,varargin(:)),repmat(A.den,varargin(:)),A.dt)
            return
        else
            execstr("B=%"+typeof(A)+"_repmat(A,varargin(:))")
            return
        end
    end

    if narg==1 then
        if size(varargin(1),"*")==1 then
            siz=list(varargin(1),varargin(1))
        else //convert array into list
            tmp=varargin(1)
            siz=list();
            for i=1:size(tmp,"*"),siz(i)=tmp(i); end
        end
    else
        siz=list();
        for i=1:narg
            siz(i)=varargin(i)
        end
    end

    nd=size(siz)
    if or(type(A)==[5 6]) then //sparse matrices
        if nd>2 then
            error(msprintf(_("%s: Wrong number of output matrix dimensions required: %d expected for sparse matrices.\n"), "repmat", 2))
        end
    end

    for i=size(siz):-1:3
        if siz(i)>1 then break,end
        nd=nd-1
    end
    sizA=size(A)
    nda=size(sizA,"*")

    if and(sizA==1) then //scalar case

        //this case can also be handled by the general one but in a less
        //efficient way
        if nd<=2 then
            B=A(ones(siz(1:nd)))
        else
            s=1;for k=1:nd;s=s*siz(k),end
            B=matrix(A(ones(s,1)),siz(1:nd))
        end
    else //general case
        if nda<nd then
            sizA(nda+1:nd)=1;
        elseif  nda>nd then
            for k=nd+1:nda
                siz(k)=1
            end
            nd=nda
        end
        I=list();
        for i=1:nd
            ind=matrix(1:sizA(i),-1,1);
            ind=ind(:,ones(1,siz(i)));
            I(i)=ind;
        end

        if nda > 2 | (size(varargin(1),"*") <> 1 & size(varargin(1)) <3) then // Works if A is hypermat but not for int8,int16 matrix
            B=A(I(:));
        else // Works for int8, int16... matrix but not for hypermat
            if rhs ==2 then
                if size(varargin(1),"*") <> 1 then // case repmat(A,size)
                    varargin_temp=varargin;
                    if size(varargin(1),1) <> 1 & size(varargin(1),2) == 1 then
                        for i=1:size(varargin(1),1)
                            varargin(i)=varargin_temp(1)(i);
                        end
                    elseif size(varargin(1),2) <> 1 & size(varargin(1),1) == 1 then
                        for i=1:size(varargin(1),2)
                            varargin(i)=varargin_temp(1)(i);
                        end
                    else
                        error(msprintf(_("%s: Wrong size for input argument #%d: a vector expected.\n"),"repmat",2));
                    end
                end
            end
            res=A(I(1),I(2));
            A_base=matrix(res,size(res,1)*size(res,2),1);
            dims=[size(A,1)*varargin(1)];
            for i=2:size(varargin) //compute dims
                dims_1=[size(A,i)*varargin(i)];
                dims=[dims, dims_1];
            end
            nb=1;
            flag=0;
            for i=3:size(varargin)
                nb=nb*varargin(i);
                if varargin(i)~=1 then // dims[2 2 1 1]=>dims[2 2]
                    flag=1;
                end
            end
            if size(varargin) ==1 then // dims[2] => dims[2 2]
                varargin(2) = varargin(1);
            end
            if flag == 0 then
                B=matrix(A_base,varargin(1)*size(A,1), varargin(2)*size(A,2));
            else
                J1=[1:size(A_base,1)];
                J=J1;
                for k=1:nb-1
                    J=[J,J1];
                end
                J=J';
                A_final=A_base(J);
                B=matrix(A_final, dims);
            end
        end
    end
endfunction

