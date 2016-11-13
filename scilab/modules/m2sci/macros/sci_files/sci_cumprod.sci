// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_cumprod(tree)
    // File generated from sci_PROTO2.g: PLEASE DO NOT EDIT !
    // M2SCI function
    // Conversion function for Matlab cumprod()
    // Input: tree = Matlab funcall tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_cumprod()

    // B = cumprod(A)
    if rhs==1 then
        A = getrhs(tree)

        // Because %b_cumprod and %C_cumprod are not defined
        A = convert2double(A)
        tree.rhs=Rhs_tlist(A)

        dim=first_non_singleton(A)

        tree.lhs(1).dims=A.dims

        if dim>0 then
            tree.rhs=Rhs_tlist(A,dim)
        elseif dim==-1 then
            tree.rhs=Rhs_tlist(A,Funcall("firstnonsingleton",1,list(A),list()))
        end
        if is_real(A) then
            tree.lhs(1).type=Type(Double,Real)
        else
            tree.lhs(1).type=Type(Double,Unknown)
        end
        // B = cumprod(A,dim)
    else
        [A,dim] = getrhs(tree)

        // Because %b_cumprod and %C_cumprod are not defined
        A=convert2double(A)
        dim=convert2double(dim)
        tree.rhs=Rhs_tlist(A,dim)

        if typeof(dim)=="cste" then
            if dim.value>size(A.dims) then
                if typeof(A)=="funcall" then
                    tree.lhs(1).infer=A.infer
                    A.lhs=tree.lhs
                elseif typeof(A)=="operation" then
                    tree.lhs(1).infer=A.infer
                    A.out=tree.lhs
                end
                tree=A;
                set_infos(msprintf(gettext("Scilab %s() does not work when dim input argument is greater than number of dims of first rhs..."),"cumprod"),1)
                return
            end
        end

        tree.name="mtlb_cumprod"
        tree.lhs(1).dims=A.dims

        if is_real(A) then
            tree.lhs(1).type=Type(Double,Real)
        else
            tree.lhs(1).type=Type(Double,Unknown)
        end
    end
endfunction
