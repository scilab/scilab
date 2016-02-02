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

function [tree]=sci_kron(tree)
    // M2SCI function
    // Conversion function for Matlab kron()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // Scilab .*. operator does no work with Strings...
    if tree.rhs(1).vtype<>Double then
        tree.rhs(1)=convert2double(tree.rhs(1))
    end
    if tree.rhs(2).vtype<>Double then
        tree.rhs(2)=convert2double(tree.rhs(2))
    end

    tree=Operation(".*.",tree.rhs,tree.lhs)

    tree.out(1).type=Type(Double,Unknown)

    if is_a_scalar(tree.operands(1)) then
        tree.out(1).dims=tree.operands(2).dims
    elseif is_a_scalar(tree.operands(2)) then
        tree.out(1).dims=tree.operands(1).dims
    end
endfunction
