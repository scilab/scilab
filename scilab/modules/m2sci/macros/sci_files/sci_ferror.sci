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

function [tree]=sci_ferror(tree)
    // M2SCI function
    // Conversion function for Matlab ferror()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    fid=getrhs(tree)

    // ferror(fid,'clear')
    if rhs==2 then
        tree.rhs(2)=null()
        if lhs==2 then
            no_equiv(msprintf(gettext("%s with two outputs."), strcat(expression2code(tree), "")));
        else
            tree.name="mclearerr"
            tree.lhs(1).dims=list(1,Unknown)
            tree.lhs(1).type=Type(String,Real)
        end
        // ferror(fid)
    else
        if lhs==2 then
            no_equiv(msprintf(gettext("%s with two outputs."), strcat(expression2code(tree), "")));
        else
            tree.name="merror"
            tree.lhs(1).dims=list(1,1)
            tree.lhs(1).type=Type(Double,Real)
        end
    end

endfunction
