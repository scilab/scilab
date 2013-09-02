// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
