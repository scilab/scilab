// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_median(tree)
    // M2SCI function
    // Conversion function for Matlab median()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_median()

    // B = median(A)
    if rhs==1 then
        A = getrhs(tree)

        // Because %b_median and %C_median are not defined
        A = convert2double(A)
        tree.rhs=Rhs_tlist(A,"m")

        if is_real(A) then
            tree.lhs(1).type=Type(Double,Real)
        else
            tree.lhs(1).type=Type(Double,Unknown)
        end

        if is_a_vector(A) | is_empty(A) then
            tree.lhs(1).dims=list(1,1)
        elseif not_a_vector(A) then
            tree.lhs(1).dims=list(1,A.dims(2))
        elseif not_empty(A) then
            dim = first_non_singleton(A)
            if dim>0 then
                tree.lhs(1).dims=A.dims
                tree.lhs(1).dims(dim)=1
            else
                tree.lhs(1).dims=allunknown(A.dims)
            end
        else
            tree.lhs(1).dims=allunknown(A.dims)
        end

        // B = median(A,dim)
    else
        [A,dim] = getrhs(tree)

        // Because %b_median and %C_median are not defined
        A=convert2double(A)
        dim=convert2double(dim)
        tree.rhs=Rhs_tlist(A,dim)

        if typeof(dim)=="cste" then
            if dim.value>size(A.dims) then
                tree.lhs(1).dims=A.dims
            else
                tree.lhs(1).dims=A.dims
                tree.lhs(1).dims(dim.value)=1
            end
        else
            tree.lhs(1).dims=allunknown(A.dims)
        end

        if is_real(A) then
            tree.lhs(1).type=Type(Double,Real)
        else
            tree.lhs(1).type=Type(Double,Unknown)
        end
    end
endfunction