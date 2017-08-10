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

function [tree]=%imp2sci(tree)
    // M2SCI function
    // Conversion function for Matlab colon
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_imp()

    // A:B
    if size(tree.operands)==2 then

        // Convert all inputs to double because Matlab also accept Strings...
        [A,B] = getoperands(tree)

        if A.vtype<>String then
            A = convert2double(A)
        end
        if B.vtype<>String then
            B = convert2double(B)
        end
        tree.operands=list(A,B)

        if  is_empty(A) | is_empty(B) then
            set_infos(msprintf(gettext("One operand is an empty matrix in : %s, result set to []."),expression2code(tree)),1);
            tree=Cste([])
            tree.dims=list(1,0)
        elseif not_empty(A) & not_empty(B) then
            tree.out(1).type=Type(Double,Real)
            if and([A.vtype,B.vtype]==String) then
                tree.out(1).dims=list(1,size(asciimat(A.value):asciimat(B.value),"*"))
                tree.out(1).type=Type(String,Real)
            elseif and([typeof(A),typeof(B)]=="cste") then
                tree.out(1).dims=list(1,size(A.value:B.value,"*"))
            else
                tree.out(1).dims=list(1,Unknown)
            end
        else
            tree=Funcall("mtlb_imp",1,list(A,B),tree.out)
            tree.lhs(1).dims=list(1,Unknown)
            tree.lhs(1).type=Type(Double,Real)
        end
        // A:inc:B
    else

        // Convert all inputs to double because Matlab also accept Strings...
        [A,inc,B]=getoperands(tree)
        if A.vtype<>String then
            A = convert2double(A)
        end
        if B.vtype<>String then
            B = convert2double(B)
        end
        if inc.vtype<>String then
            inc = convert2double(inc)
        end
        tree.operands=list(A,inc,B)

        if is_empty(A) | is_empty(B) | is_empty(inc) then
            set_infos(msprintf(gettext("One operand is an empty matrix in : %s, result set to []."),expression2code(tree)),1);
            tree=Cste([])
            tree.dims=list(1,0)
        elseif not_empty(A) & not_empty(B) & not_empty(inc) then
            tree.out(1).dims=list(1,Unknown)
            tree.out(1).type=Type(Double,Real)
        else
            tree=Funcall("mtlb_imp",1,list(A,inc,B),tree.out)
            tree.lhs(1).dims=list(1,Unknown)
            tree.lhs(1).type=Type(Double,Real)
        end
        if and([A.vtype,B.vtype]==String) then
            tree.out(1).type=Type(String,Real)
        end
    end
endfunction

