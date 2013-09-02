// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_num2str(tree)
    // M2SCI function
    // Conversion function for Matlab num2str()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if rhs==1 then
        set_infos(gettext("string output can be different from Matlab num2str output."),2)
        tree.name="string"
    else
        if tree.rhs(2).vtype==String then
            tree.name="msprintf"
            tree.rhs=Rhs_tlist(tree.rhs(2),tree.rhs(1))
        elseif tree.rhs(2).vtype<>Unknown then
            no_equiv(expression2code(tree))
            set_infos(gettext("See msprintf for solutions."),1);
        else
            tree.name="mtlb_num2str"
        end
    end
endfunction
