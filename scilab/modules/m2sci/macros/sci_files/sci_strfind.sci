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

function [tree]=sci_strfind(tree)
    // M2SCI function
    // Conversion function for Matlab strfind()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_strfind()

    [A,B] = getrhs(tree)

    if A.vtype==String & B.vtype==String then
        tree.name="strindex"
        tree.lhs(1).dims=list(Unknown,Unknown)
        tree.lhs(1).type=Type(Double,Real)
    else
        tree.name="mtlb_strfind"
        tree.lhs(1).dims=list(Unknown,Unknown)
        tree.lhs(1).type=Type(Double,Real)
    end
endfunction
