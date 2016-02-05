// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - S. Steer, F. Delebecque, V. Couvert
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function  M=createstruct(index,N)
    // Create a struct with field pointed by index equal to N

    // If index is not a list, creation is done by routines
    if type(index)==15 then

        // When only two indexes
        if size(index)==2 then

            // Second index is a fieldname
            if type(index(2))==10 then

                // First index is a numerical value or a vector of numerical values
                if type(index(1))==1 then
                    // X(p).f = y  -> index=list(p,f)
                    I=index(1)
                    if mtlb_mode() then
                        Dims=[1 max(I)]
                    else
                        Dims=[max(I) 1]
                    end
                    nmax=prod(Dims);
                    if nmax==1 then
                        M=mlist(["st","dims",index(2)],int32(Dims),N);
                    else
                        M=mlist(["st","dims",index(2)],int32(Dims));
                        //initialize the structure with empty fields
                        Li=list();for kl=1:nmax, Li(kl)=[];end
                        //set fields pointed to by index(1) to N
                        for kl=1:size(I,"*"), Li(I(kl))=N;end
                        M=setfield(3,Li,M);
                    end
                    return;
                    // First index is a list of numerical values
                elseif type(index(1))==15
                    // X(p,q[,...]).f=z -> index=list(list(p,q[,...]),f)
                    I=index(1)
                    Ndims=lstsize(I) //the number of dimensions

                    //compute the struct dimensions from max indices values
                    Dims=[];for kDims=1:Ndims, Dims=[Dims,max(I(kDims)) ];end

                    //initialize the structure with empty fields
                    Li=list();for kl=1:prod(Dims),Li(kl)=[];end

                    //set fields pointed to by I to N
                    I1=sub2ind(Dims,I(:)) // build one dimensional index equivalent to I
                    for kl=1:size(I1,"*"), Li(I1(kl))=N;end

                    M=mlist(["st","dims",index(2)],int32(Dims),Li);
                    return;
                    // First index is also a fieldname
                else
                    // X.f.g=y -> index=list(f,g)
                    M=mlist(["st","dims",index(1)],int32([1,1]),...
                    mlist(["st","dims",index(2)],int32([1,1]),N));
                    return;
                end

                // Second index is a fieldname
            else
                // X.f(p[,q,...])=y : index=list(f,list(p[,q,...]))

                if typeof(N)=="st" then // When recursive call of createstruct
                    if type(index(2))==15 then // index=list(f,list(p,q[,...]))
                        Dims=list2vec(index(2))'
                    else // index=list(f,p)
                        if mtlb_mode() then
                            Dims=[1 index(2)]
                        else
                            Dims=[index(2) 1]
                        end
                    end
                    kmax=prod(Dims)
                    z=list()
                    for kl=1:kmax
                        z(kl)=[]
                    end
                    z(kmax)=getfield(3,N)
                    z=mlist(getfield(1,N),int32(Dims),z);
                else
                    z(index(2)(:))=N;
                end
                if type(index(1))<>10 then pause,end
                M=mlist(["st","dims",index(1)],int32([1,1]),z);
                return;
            end

            // Any number of indexes <> 2
        else
            // Last index is a fieldname -> init struct
            if type(index($))==10 then
                M=mlist(["st","dims",index($)],int32([1,1]),N);
                index($)=null();
                // Last index is a numerical value or a list of numerical values
            elseif type(index($))==1 | type(index($))==15 then

                // If only one index value (happens when recursive calls to createstruct)
                if size(index)==1 then
                    if typeof(N)=="st" then
                        if type(index($))==15 then
                            Dims=list2vec(index($))'
                        else
                            if mtlb_mode() then
                                Dims=[1 index($)]
                            else
                                Dims=[index($) 1]
                            end
                        end
                        kmax=prod(Dims)
                        if kmax==1 then
                            M=N
                        else
                            z=list()
                            for kl=1:kmax
                                z(kl)=[]
                            end
                            z(kmax)=getfield(3,N)
                            M=mlist(getfield(1,N),int32(Dims),z);
                        end
                    else
                        M(index($)(:))=N;
                    end
                    index($)=null()
                    // More than one index value
                else
                    if typeof(N)=="st" then
                        if type(index($))==15 then
                            Dims=list2vec(index($))'
                        else
                            if mtlb_mode() then
                                Dims=[1 index($)]
                            else
                                Dims=[index($) 1]
                            end
                        end
                        kmax=prod(Dims)
                        z=list()
                        for kl=1:kmax
                            z(kl)=[]
                        end
                        z(kmax)=getfield(3,N)
                        z=mlist(getfield(1,N),int32(Dims),z);
                    else
                        z(index($)(:))=N;
                    end
                    M=mlist(["st","dims",index($-1)],int32([1,1]),z);
                    index($)=null()
                    index($)=null()
                end
            else
                // This case should not happen (unknown kind of index)
                error(msprintf(_("%s: Not implemented.\n"),"createstruct"));
            end
            if index==list() then
                return
            end
            M=createstruct(index,M);
            return;
        end
    else
        if type(index)==10 then
            M=mlist(["st","dims",index($)],int32([1,1]),N);
        else
            error(msprintf(_("%s: Not implemented.\n"),"createstruct"));
        end
    end
endfunction
