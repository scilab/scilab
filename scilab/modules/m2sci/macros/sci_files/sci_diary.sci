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

function [tree]=sci_diary(tree)
    // M2SCI function
    // Conversion function for Matlab diary()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // diary toggle
    if rhs<=0 then
        no_equiv("diary toggle")
    else
        str=getrhs(tree)
        if typeof(str)=="variable" then
            str=str.name
        elseif typeof(str)=="cste" then
            str=str.value
        end
        if convstr(str,"l")=="""off""" then // diary off
            tree.rhs=Cste(0)
        elseif convstr(str,"l")=="""on""" then // diary on
            no_equiv("diary on")
        else // diary(filename)
            // Nothing to do
        end
    end
endfunction
