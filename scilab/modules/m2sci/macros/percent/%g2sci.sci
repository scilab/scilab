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

function tree = %g2sci(tree)
    // M2SCI function
    // Conversion function for Matlab logical OR
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree

    // Overloading functions in $SCI/modules/m2sci/macros/compat_functions/:
    // - %b_g_s.sci
    // - %s_g_b.sci
    // These functions are not used to get the same output value as Matlab one with empty matrices

    [A,B] = getoperands(tree)

    // To have good size for result with String as input
    // And overloading functions are not written for Strings
    if A.vtype==Unknown | A.vtype==String then
        A = convert2double(A)
    end
    if B.vtype==Unknown | B.vtype==String then
        B = convert2double(B)
    end
    tree.operands = list(A,B)

    tree.out(1).type = Type(Boolean,Boolean)
    // If A is a scalar
    if is_a_scalar(A) then
        tree.out(1).dims = B.dims
        // If B is a scalar
    elseif is_a_scalar(B) then
        tree.out(1).dims = A.dims
        // If A or B is an empty matrix
    elseif is_empty(A) | is_empty(B) then
        tree.out(1).dims = A.dims
        // A and B are not scalars and not empty matrices -> they have the same size
    elseif not_empty(A) & not_empty(B) then
        tree.out(1).dims = A.dims
    else
        tree.out(1).dims = allunknown(A.dims)
    end

endfunction
