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

function [tree]=sci_strrep(tree)
    // M2SCI function
    // Conversion function for Matlab strrep()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_strrep()

    [s1,s2,s3] = getrhs(tree)

    if s1.vtype==String & s2.vtype==String & s3.vtype==String then
        tree.name="strsubst"
        tree.lhs(1).dims=list(Unknown,Unknown)
        tree.lhs(1).type=Type(String,Real)
    else
        tree.name="mtlb_strrep"
        tree.lhs(1).dims=list(Unknown,Unknown)
        tree.lhs(1).type=Type(Unknown,Unknown)
    end
endfunction
