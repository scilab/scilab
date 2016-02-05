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
