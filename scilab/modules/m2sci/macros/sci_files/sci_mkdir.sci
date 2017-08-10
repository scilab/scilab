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

function [tree]=sci_mkdir(tree)
    // M2SCI function
    // Conversion function for Matlab mkdir
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    tree.lhs(1).dims=list(1,1);
    tree.lhs(1).type=Type(Double,Real);

    if lhs>=2 then
        tree.lhs(2).dims=list(1,Unknown);
        tree.lhs(2).type=Type(String,Real);
    end

    if lhs==3 then
        no_equiv(msprintf(gettext("%s when called with three outputs."), strcat(expression2code(tree), "")));
    end

endfunction
