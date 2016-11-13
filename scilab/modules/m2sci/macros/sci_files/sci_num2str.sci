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
