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

function [tree]=sci_generic(tree)
    // M2SCI function
    // Generic conversion function for unknown Matlab functions
    // Input: tree = Matlab funcall tree
    // Output: tree = Scilab equivalent for tree

    if typeof(tree)=="operation"
        tree.out(1).dims=list(-1,-1)
        tree.out(1).type=Type(-1,-1)
    else
        for i=1:size(tree.lhs)
            tree.lhs(i).dims=list(-1,-1)
            tree.lhs(i).type=Type(-1,-1)
        end
    end
endfunction
