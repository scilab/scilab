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

function [tree]=sci_eval(tree)
    // M2SCI function
    // Conversion function for Matlab eval()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_eval()

    if lhs==1 then
        // eval(expression)
        if rhs==1 then
            tree.name="mtlb_eval"
            // eval(expression,catch_expr)
        else
            tree.name="mtlb_eval"
        end
    else
        tree.name="mtlb_eval"

        // lhs names
        LHS=[]
        for k=1:lhs
            LHS=[LHS,tree.lhs(k).name]
        end
        LHS="["+strcat(LHS,",")+"]"
        if typeof(tree.rhs(1))=="cste" then
            tree.rhs=Rhs_tlist(LHS+" = "+tree.rhs(1).value)
        else
            tree.rhs=Rhs_tlist(LHS+" = "+expression2code(tree.rhs(1)))
        end
        tree.lhs=list(Variable("ans",Infer()))
    end
endfunction
