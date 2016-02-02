// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - F. Delebcque , Serge Steer
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function M=%st_i_st(varargin)

    //insert the struct varargin($-1) into the struct varargin($)
    //at position varargin(1), varargin(2),varargin(3),...
    //insert the struct varargin($-1) as the field varargin(1)
    //of the struct varargin($)

    [lhs,rhs]=argn(0)
    M=varargin($)
    N=varargin($-1)//inserted matrix
    dims=double(matrix(M.dims,1,-1));

    if type(varargin(1))==10 //addind a new field
        flds=getfield(1,M);flds=[flds,varargin(1)];
        M=setfield(1,flds,M);M=setfield($+1,N,M);
        if and(dims==[0 0]) then M.dims=int32([1 1]),end
    else   //Subscripted assignment between structures
        //build resulting struct fields
        FM=getfield(1,M);FM=FM(3:$);nFM=size(FM,"*")
        FN=getfield(1,N);FN=FN(3:$);nFN=size(FN,"*")
        FR=FM;for f=FN, if and(f<>FM) then FR=[FR,f],end,end

        Nfields=size(FR,"*")
        //computing the dimension of the result
        nd=size(dims,"*")

        olddims=dims

        reduced_index=%f
        if rhs-2>nd then  //more indices than M number of dims
            dims(nd+1:rhs-2)=1;
        elseif rhs-2<nd  then //less indices than M number of dims
            dims=[dims(1:rhs-3) prod(dims(rhs-2:$))]
            if size(find(dims>1),"*")>1 then reduced_index=%t,end
        end
        //convert N-dimensional indexes to 1-D and extend dims if necessary
        [Ndims,I]=convertindex(list(dims,double(matrix(N.dims,1,-1))),varargin(1:$-2));Ndims=matrix(Ndims,1,-1)
        if reduced_index&or(Ndims<>dims)  then error(21),end
        if or(Ndims>dims) then
            //extend the destination matrix
            I1=0
            for k=size(Ndims,"*"):-1:1
                ik1=(1:dims(k))';
                if ik1<>[] then
                    if Ndims(k)>1 then
                        if size(I1,"*")>1 then
                            I1=(Ndims(k)*I1).*.ones(ik1)+ones(I1).*.(ik1-1);
                        else
                            I1=Ndims(k)*I1+ik1-1;
                        end
                    else
                        I1=Ndims(k)*I1+ik1-1;
                    end
                end
            end
            v1=list();for k=1:prod(Ndims),v1(k)=[];end
            // create the resulting matrix
            R=mlist(["st","dims",matrix(FR,1,-1)],int32(Ndims));
            for k=1:size(FR,"*"),R=setfield(2+k,v1,R),end
            // populate it with M entries
            for k=1:nFM
                v2=v1;
                kf=find(FR==FM(k));
                w=getfield(k+2,M);
                if type(w)<>15 then w=list(w),end
                if type(v2)<>15 then v2=list(v2);end
                for i=1:size(I1,"*"),
                    if w<>list() then
                        v2(I1(i)+1)=w(i);
                    else
                        v2(I1(i)+1)=[];
                    end
                end
                R=setfield(kf+2,v2,R);
            end
        else //the dimension agree
            R=M
            //does the fields agree?
            if or(FR<>FM) then //no
                //add new fields
                R=setfield(1,["st","dims",FR],R)
                v1=list();for k=1:prod(Ndims),v1(k)=[];end
                for k=nFM+1:size(FR,"*")
                    R=setfield($+1,v1,R)
                end
            end
        end

        //insert N entries into result
        for k=1:nFN
            kf=find(FR==FN(k));
            v2=getfield(kf+2,R);
            w=getfield(k+2,N);
            if type(w)<>15 then w=list(w),end
            if type(v2)<>15 then v2=list(v2);end
            for i=1:size(I,"*"),
                if w<>list() then
                    if size(w)==1 then
                        v2(I(i))=w(1);
                    else
                        v2(I(i))=w(i);
                    end
                else
                    v2(I(i))=[];
                end
            end
            if length(v2)==1 then v2=v2(1);end
            R=setfield(kf+2,v2,R);
        end

        //remove trailing unitary dimensions
        if reduced_index then
            Ndims=olddims
        else
            while  Ndims($)==1 then Ndims($)=[],end
            select size(Ndims,"*")
            case 0 then
                Ndims=[1,1]
            case 1 then
                k=find(olddims<>1&olddims<>0)
                if k==[] // M was a scalar
                    if mtlb_mode() then
                        Ndims=[1,Ndims]
                    else
                        Ndims=[Ndims,1]
                    end
                else // M was a vector
                    olddims(k)=Ndims
                    Ndims=olddims;
                end
            else
                Ndims=matrix(Ndims,1,-1)
            end
        end
        R.dims=int32(Ndims)
        M=R
    end
endfunction
