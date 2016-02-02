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

function [tree]=sci_feval(tree)
    // M2SCI function
    // Conversion function for Matlab feval()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_feval()

    set_infos(gettext("Verify that expression evaluated by execstr() is Scilab compatible."),2)

    RHS="("
    for k=2:rhs
        if k<>2 then
            RHS=RHS+","
        end
        if typeof(tree.rhs(k))=="cste" then
            RHS=RHS+string(tree.rhs(k).value)
        else
            RHS=RHS+rhs2code(tree.rhs(k))
        end
    end
    RHS=RHS+")"
    RHS=Operation("+",list(tree.rhs(1),Cste(RHS)),list())

    if lhs==1 then
        tree.name="evstr"
        tree.rhs=list(RHS)
    else
        tree.name="execstr"
        LHS="["
        for k=1:lhs
            if k>1 then
                LHS=LHS+","
            end
            LHS=LHS+tree.lhs(k).name
        end
        LHS=LHS+"]"

        tree.rhs=Rhs_tlist(Operation("+",list(Cste(LHS+" = "),RHS),list()))

        tree.lhs=list(Variable("ans",Infer()))
    end
endfunction
