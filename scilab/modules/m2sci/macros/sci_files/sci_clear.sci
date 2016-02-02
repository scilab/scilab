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

function [tree]=sci_clear(tree)
    // M2SCI function
    // Conversion function for Matlab clear()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    for k=1:rhs
        if tree.rhs(k).value=="" then
            tree.rhs(k)=null()
        end
    end

    if size(tree.rhs)==0 then
        tree.lhs(1).dims=list(Unknown,Unknown)
        tree.lhs(1).type=Type(Unknown,Unknown)
    else
        select tree.rhs(1).value
        case "global" then
            tree.rhs(1)=null()
            tree.name="clearglobal"
        case "all" then
            tree.rhs=list()
        case "classes" then
            no_equiv(expression2code(tree))
        case "functions" then
            no_equiv(expression2code(tree))
        case "import" then
            no_equiv(expression2code(tree))
        case "variables" then
            tree.rhs=list()
        else
            // Nothing to do
        end
        tree.lhs(1).dims=list(Unknown,Unknown)
        tree.lhs(1).type=Type(Unknown,Unknown)
    end

endfunction
