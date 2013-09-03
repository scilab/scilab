// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_toeplitz(tree)
    // M2SCI function
    // Conversion function for Matlab toeplitz()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // T = toeplitz(c)
    if rhs==1 then
        c=tree.rhs(1)
        if c.property<>Real then
            tree.name="mtlb_toeplitz"
        elseif ~is_a_vector(c) & ~is_a_scalar(c) then
            tree.name="mtlb_toeplitz"
        end
        tree.rhs(1)=c
        tree.lhs(1).type=Type(Double,c.property)
        // T = toeplitz(c,r)
    else
        [c,r]=getrhs(tree)
        tree.rhs=Rhs_tlist(c, r)
        tree.name="mtlb_toeplitz"
        if c.property==Complex | r.property==Complex then
            tree.lhs(1).type=Type(Double,Complex)
        elseif c.property==Real | r.property==Real then
            tree.lhs(1).type=Type(Double,Real)
        else
            tree.lhs(1).type=Type(Double,Unknown)
        end
    end

endfunction
