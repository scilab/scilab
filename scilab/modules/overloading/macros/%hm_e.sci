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

function [M] = %hm_e(varargin)

    // version modified by Bruno
    // extract an sub_hypermatrix
    [lhs,rhs]=argn(0)
    M=varargin(rhs)
    dims=double(M.dims)   // new hypermatrices have an int32 dims field
    nbdims = size(dims,"*");
    nb_index_vect = rhs-1;

    if nb_index_vect > nbdims then
        error(msprintf(_("%s: Too many subscripts.\n"),"%hm_e"));
    elseif nb_index_vect < nbdims then
        nbdims = nb_index_vect;
        dims(nbdims) = prod(dims(nbdims:$))
    end

    I = 0
    dims1 = [];

    for k=nb_index_vect:-1:1
        ik=varargin(k)
        type_ik = type(ik)
        if type_ik==1 then                    // usual subscript
            if min(size(ik))<0 then
                if rhs==2 then // subscript is : in fact
                    M=matrix(M.entries,-1,1)
                    return
                end
                ik=1:dims(k)
            else
                ik=round(ik)
                if min(ik) <= 0 then, error(21), end
                if max(ik) > dims(k) then, error(21), end
            end
        elseif type_ik==2 then                // poly subscript
            ik=round(horner(ik,dims(k)))
            if min(ik) <= 0 then, error(21), end
            if max(ik) > dims(k) then, error(21), end
        elseif type_ik == 129 then            // implicit poly subscript (p.e. 1:$)
            ik=round(horner(ik(:),dims(k)))
            if min(ik(1),ik(3)) <= 0 then, error(21), end
            if max(ik(1),ik(3)) > dims(k) then, error(21), end
            ik = ik(1):ik(2):ik(3)
        elseif type_ik==4 | type_ik==6 then   // boolean and sparse boolean subscript
            if size(ik,"*") ~= dims(k) then, error(21), end
            ik=find(ik)
        elseif typeof(ik) == "hypermat" then  // hm boolean subscript
            if type(ik.entries) ~= 4 then, error(21), end
            if size(ik,"*") ~= dims(k) then, error(21), end
            ik=find(ik.entries)
        else
            error(21)
        end

        nik = size(ik, "*");
        if (nik == 0) then, M=[], return, end
        dims1 = [nik, dims1]
        if nik > 1 then
            ik=ik(:)
            if size(I,"*") > 1 then
                I=(dims(k)*I).*.ones(ik)+ones(I).*.(ik-1)
            else
                I=dims(k)*I+ik-1
            end
        else
            I=dims(k)*I+ik-1
        end
    end
    //
    dims1(max(find(dims1>1))+1:$)=[]  // elimine la fin si les dims sont 1
    // exemple dims1 = [2 4 1 6 7 1 1] alors on obtient dims1 = [2 4 1 6 7]
    // (pour compatibilite avec Matlab)

    select size(dims1,"*")
    case 0
        M = M.entries(I+1)
    case 1
        M = M.entries(I+1)
    case 2
        M = matrix(M.entries(I+1),dims1(1),dims1(2))
    else
        M = mlist(["hm","dims","entries"],int32(dims1),M.entries(I+1))
    end

endfunction

