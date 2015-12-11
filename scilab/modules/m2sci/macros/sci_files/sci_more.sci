// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_more(tree)
    // M2SCI function
    // Conversion function for Matlab more()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_more()

    opt=getrhs(tree)

    tree.name="lines"
    if typeof(opt)=="cste" then
        if opt.value=="off" then
            tree.rhs=Rhs_tlist(0)
        elseif opt.value=="on" then
            tree.rhs=Rhs_tlist(60)
        else
            // Nothing to do
        end
    else
        tree.name="mtlb_more"
    end

endfunction
