// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_full(tree)
    // M2SCI function
    // Conversion function for Matlab full()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_full()

    S = getrhs(tree)

    if S.vtype==String then
        tree.name="mtlb_full"
        tree.lhs(1).dims=S.dims
        tree.lhs(1).type=S.type
    elseif S.vtype==Boolean then
        S = convert2double(S)
        tree.rhs=Rhs_tlist(S)
        tree.lhs(1).dims=S.dims
        tree.lhs(1).type=Type(Boolean,Real)
    elseif S.vtype==Double then
        tree.lhs(1).dims=S.dims
        tree.lhs(1).type=S.type
    elseif S.vtype==Sparse then
        tree.lhs(1).dims=S.dims
        tree.lhs(1).type=Type(Double,S.property)
    else
        tree.name="mtlb_full"
        tree.lhs(1).dims=S.dims
        tree.lhs(1).type=S.type
    end

endfunction
