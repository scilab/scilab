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

function [tree]=sci_global(tree)
    // M2SCI function
    // Conversion function for Matlab global()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // Global variable for M2SCI
    global("varslist");

    for k=1:rhs
        nam=tree.rhs(k).value
        if nam<>"" then
            exist=%F
            for l=1:size(varslist)
                if varslist(l).matname==nam then
                    exist=%T
                    break
                end
            end
            if exist==%F then
                varslist($+1)=M2scivar(nam,nam,Infer(list(Unknown,Unknown),Type(Unknown,Unknown)))
                write(%io(2),msprintf(gettext("Global variable %s added to M2SCI variable list."),nam))
            end
        end
    end

endfunction
