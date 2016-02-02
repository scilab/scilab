// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) DIGITEO - 2011 - Allan CORNET
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [Newdims,I] = convertindex(dims, varargin)
    //convert N-D indexes given in varargin to 1-D index.
    //if indexes exceed matrix dimensions given in dims, dimensions are
    //extented and new dimensions are returned in Newdims

    // VC 27/05/2004
    // if dims is a list (INSERTION) it contains:
    //  1) dims: vector of dimensions of matrix where data are inserted
    //  2) Ndims: vector of dimensions of matrix to insert
    // else (EXTRACTION) is is a vector (vector of dimensions)

    // Modified files: %ce_i_ce.sci %st_i_st.sci %b_i_hm.sci %hm_i_b.sci %hm_i_hm.sci %hm_i_i.sci %hm_i_p.sci %hm_i_s.sci %i_i_hm.sci %p_i_hm.sci

    rhs = argn(2);
    if rhs < 1 then
        error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"),"convertindex", 1));
    end

    if type(dims)==15 then // insertion
        Ndims=dims(2)
        dims=dims(1)
    else // extraction
        Ndims=[]
    end

    Newdims=[]
    I=0
    iimp=0
    for k=size(varargin):-1:1
        ik=varargin(k)
        if type(ik)==2 |type(ik)==129 then // size implicit subscript $...
            ik=horner(ik,dims(k)) // explicit subscript
            Newdims(k,1)=max(max(ik),dims(k))
        elseif type(ik)==4 then // boolean subscript
            ik=find(ik)
            Newdims(k,1)=max(max(ik),dims(k))
        elseif min(size(ik))<0 then // :
            if dims(k)<>0 then
                ik=1:dims(k)
            else
                iimp=iimp+1
                if iimp<=size(Ndims,"*") then
                    if isempty(Ndims) then // extraction
                        ik=[]
                    else // insertion
                        if iimp==1 then
                            single=%t
                            for kk=1:k-1
                                if size(varargin(kk),1)==-1|size(varargin(kk),"*")>1 then
                                    single=%f
                                    break
                                end
                            end
                            if single then
                                ik=1:prod(Ndims),
                            else
                                ik=1:Ndims($+1-iimp)
                            end
                        else
                            ik=1:Ndims($+1-iimp)
                        end
                    end
                else
                    ik=1
                end
            end
            Newdims(k,1)=max([max(ik),dims(k)])
        else //floating point subscript
            ik=round(ik)
            Newdims(k,1)=max(max(ik),dims(k))
        end

        if size(ik,"*")>1 then
            ik=ik(:)
            if size(I,"*")>1 then
                I=(Newdims(k)*I).*.ones(ik)+ones(I).*.(ik-1);
            else
                I=Newdims(k)*I+ik-1;
            end
        else
            I=Newdims(k)*I+ik-1;
        end
    end
    I=I+1
endfunction
