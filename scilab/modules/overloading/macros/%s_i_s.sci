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

function M=%s_i_s(varargin)

    //insertion of a matrix in an hypermatrix
    [lhs,rhs]=argn(0)
    M=varargin(rhs)
    N=varargin(rhs-1)//inserted matrix

    index=varargin(1) //
    if rhs==3 then
        //result will be a cell a struct
        if type(index)==10 then //M.x=N or M.entries=N
            M=struct()
            M(index)=N
            if index=="entries" then //M.entries=N

                // change struct to cell
                f=getfield(1,M);f(1)="ce"
                M=setfield(1,f,M)
            end
            return
        elseif type(index)==15 then
            //M(i).x=N or M(i,j,..).x=N or M.x(i,j,..)or M(i,j..)
            //check for a name in the index list
            isstr=%f; for ii=index,if type(ii)==10 then  isstr=%t,break,end,end
            if isstr then
                M=createstruct(index,N)
                if type(index(1))<>10 & index(2)=="entries" then
                    // change struct to cell
                    f=getfield(1,M);f(1)="ce"
                    M=setfield(1,f,M)
                end
            else
                M(index(:))=N
            end
            return
        end
    end

    //X(i,j,k)=n  hypermatrix
    dims=matrix(size(M),-1,1)
    v=M(:)

    Ndims=rhs-2
    nd=size(dims,"*")
    if Ndims>nd then dims(nd+1:Ndims)=0;end
    del=N==[];count=[]
    dims1=[]
    I=0;I1=0
    iimp=0
    for k=Ndims:-1:1
        ik=varargin(k)//the kth subscript
        if type(ik)==2 |type(ik)==129 then // size implicit subscript $...
            ik=round(horner(ik,dims(k))) // explicit subscript
            dims1(k,1)=max(max(ik),dims(k))
        elseif type(ik)==4 then // boolean subscript
            ik=find(ik)
            dims1(k,1)=max(max(ik),dims(k))
        elseif min(size(ik))<0 then // :
            if dims(k)<>0 then
                ik=1:dims(k)
            else
                iimp=iimp+1

                if iimp<=size(size(N),"*") then

                    if iimp==1 then
                        single=%t
                        for kk=1:k-1
                            if size(varargin(kk),1)==-1|size(varargin(kk),"*")>1 then
                                single=%f
                                break
                            end
                        end
                        if single then
                            ik=1:size(N,"*"),
                        else
                            ik=1:size(N,size(size(N),"*")+1-iimp)
                        end
                    else
                        ik=1:size(N,size(size(N),"*")+1-iimp)
                    end
                else
                    ik=1
                end
            end
            dims1(k,1)=max(max(ik),dims(k))
            if k==Ndims then
                if k<nd then
                    ik=1:prod(dims(k:$))
                    dims1(k:nd,1)=dims(k:nd)
                end
            end
        else //floating point subscript
            ik=round(ik)
            dims1(k,1)=max(max(ik),dims(k))
        end


        if size(ik,"*")>1 then
            ik=ik(:)
            if size(I,"*")>1 then
                I=(dims1(k)*I).*.ones(ik)+ones(I).*.(ik-1)
            else
                I=dims1(k)*I+ik-1
            end
        else
            I=dims1(k)*I+ik-1
        end
        if del then
            if or(ik<>(1:dims1(k))') then
                count=[count k]
                nk=size(ik,"*")
            end
        end
    end //end of the loop on subscripts

    if ~del&or(dims1>dims) then
        I1=0
        for k=size(dims1,"*"):-1:1
            ik1=(1:dims(k))'
            if ik1<>[] then
                if dims1(k)>1 then
                    if size(I1,"*")>1 then
                        I1=(dims1(k)*I1).*.ones(ik1)+ones(I1).*.(ik1-1)
                    else
                        I1=dims1(k)*I1+ik1-1
                    end
                else
                    I1=dims1(k)*I1+ik1-1
                end
            end
        end
        v1=zeros(prod(dims1),1)
        v1(I1+1)=v;v=v1
    end
    v(I+1)=matrix(N,-1,1)
    if del then
        if size(count,"*")>1 then
            error(msprintf(_("%s: A null assignment can have only one non-colon index.\n"),"%s_i_s"));
        end
        dims1(count)=dims1(count)-nk
    end

    while  dims1($)==1 then dims1($)=[],end
    select size(dims1,"*")
    case 0
        M=v
    case 1
        M=v
    case 2
        M=matrix(v,dims1(1),dims1(2))
    else
        M=matrix(v, dims1)
    end
endfunction
