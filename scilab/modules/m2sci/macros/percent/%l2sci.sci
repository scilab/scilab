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

function [tree]=%l2sci(tree)
    // M2SCI function
    // Conversion function for Matlab left division
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_l()

    [A,B] = getoperands(tree)

    // We have to call mtlb_l() for strings because result is computed with transposed strings...
    if or(A.vtype==[String,Unknown]) & or(A.vtype==[String,Unknown]) then
        tree=Funcall("mtlb_l",1,list(A,B),tree.out)
        tree.lhs(1).dims=allunknown(A.dims)
        if A.vtype==String & B.vtype==String then
            tree.lhs(1).type=Type(Double,Real)
        else
            tree.lhs(1).type=Type(Double,Unknown)
        end
    else
        // Boolean -> Double
        A = convert2double(A)
        B = convert2double(B)

        tree.operands=list(A,B)

        if is_real(A) & is_real(B) then
            tree.out(1).type=Type(Double,Real)
        elseif (is_real(A) & is_complex(B)) | (is_real(B) & is_complex(A)) then
            tree.out(1).type=Type(Double,Complex)
        else
            tree.out(1).type=Type(Double,Unknown)
        end

        if is_a_scalar(A) then
            tree.out(1).dims=B.dims
        elseif is_a_scalar(B) then
            tree.out(1).dims=A.dims
        else
            tree.out(1).dims=list(A.dims(2),B.dims(2))
        end
    end
endfunction

