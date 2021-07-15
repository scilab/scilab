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

function tree = %log2sci(tree)
    // M2SCI function
    // Conversion function for Matlab logical operators
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_logic()

    [A, B] = getoperands(tree)
    if or(A.type==[Boolean String]) then
        A = convert2double(A)
    end
    if or(B.type==[Boolean String]) then
        B = convert2double(B)
    end

    // Special case for nargout
    if typeof(A)=="variable" & typeof(B)=="cste" then
        if A.name=="nargout" & B.value==0 then
            B = Cste(1)
        end
    end
    if typeof(B)=="variable" & typeof(A)=="cste" then
        if B.name=="nargout" & A.value==0 then
            A = Cste(1)
        end
    end

    tree.operands = list(A,B)

    // Scilab operators >, <, >= and <= do not work with complex values
    if or(tree.operator==["<", ">", "<=", ">="]) & (~is_real(A) | ~is_real(B)) then
        tree = Funcall("mtlb_logic",1,list(A,Cste(tree.operator),B),tree.out)
        if is_a_scalar(A)
            tree.lhs(1).dims = B.dims
        elseif is_a_scalar(B) | and(A.dims==B.dims)
            tree.lhs(1).dims = A.dims
        end
    else
        // Cases with empty matrix
        if is_empty(A) | is_empty(B) then
            // For == and ~= : Scilab returns %T or %F
            tree = Cste([])
        elseif is_a_scalar(A) & not_empty(B) then
            tree.out(1).dims = B.dims
        elseif is_a_scalar(B) & not_empty(A) then
            tree.out(1).dims = A.dims
        elseif not_empty(A) & not_empty(B) then
            tree.out(1).dims = A.dims
        else
            tree = Funcall("mtlb_logic",1,list(A,Cste(tree.operator),B),tree.out)
            if is_a_scalar(A)
                tree.lhs(1).dims = B.dims
            elseif is_a_scalar(B) | and(A.dims==B.dims)
                tree.lhs(1).dims = A.dims
            end
            tree.lhs(1).dims = A.dims
        end
    end

    // Result's property
    // -----------------
    if is_sparse(A)>0 | is_sparse(B)>0
        tree.type = Type(Boolean, Sparse)
    else
        [tA, tB] = (A.type, B.type)
        if tA.vtype==Unknown | tB.vtype==Unknown |..
           (tA.vtype==Boolean & tA.property==Unknown) | ..
           (tB.vtype==Boolean & tB.property==Unknown)
            tree.type = Type(Boolean,Unknown)
        else
            tree.type = Type(Boolean,Boolean)
        end
    end

endfunction
