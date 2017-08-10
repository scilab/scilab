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

function [tree]=%m2sci(tree)
    // M2SCI function
    // Conversion function for Matlab multiplication
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree

    // Overloading functions in $SCI/modules/compatibility_functions/macros/:
    // - %b_m_s.sci
    // - %s_m_b.sci

    [A,B]=getoperands(tree)

    // Multiplication does not work with Strings in Scilab
    if or(A.vtype==[String,Unknown]) then
        A=convert2double(A)
    end
    if or(B.vtype==[String,Unknown]) then
        B=convert2double(B)
    end

    // %b_m_b is not defined in Scilab
    if A.vtype==Boolean & B.vtype==Boolean then
        B=convert2double(B)
    end
    tree.operands=list(A,B)

    if is_complex(A) & is_complex(B) then
        prop=Unknown
    elseif A.property==Complex | B.property==Complex then
        if not_empty(A) & not_empty(B) then
            prop=Complex
        elseif is_empty(A) | is_empty(B) then
            prop=Real
        else
            prop=Unknown
        end
    elseif is_real(A) & is_real(B) then
        prop=Real
    else
        prop=Unknown
    end

    tree.out(1).type=Type(Double,prop)

    if is_a_scalar(A) then
        tree.out(1).dims=B.dims
    elseif is_a_scalar(B) then
        tree.out(1).dims=A.dims
    elseif not_a_scalar(A) & not_a_scalar(B) then
        // A and B have the same dimensions
        tree.out(1).dims=A.dims;
        for kk=1:size(B.dims)
            if B.dims<>Unknown then
                tree.out(1).dims(kk) = B.dims(kk);
            end
        end
    elseif not_empty(A) & not_empty(B) then
        tree.out(1).dims=list(A.dims(1),B.dims(2))
    elseif is_empty(A) | is_empty(B) then
        tree.out(1).dims=list(0,0)
    elseif not_empty(A) | not_empty(B) then
        tree.out(1).dims=list(A.dims(1),B.dims(2))
    else
        tree.out(1).dims=list(Unknown,Unknown)
    end
endfunction
