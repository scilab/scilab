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

function [tree]=sci_save(tree)
    // M2SCI function
    // Conversion function for Matlab save()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    tree.name="savematfile"

    // If no rhs, save Scilab environment
    if rhs<=0 then
        tree.rhs=Rhs_tlist("scilab.mat")
        return
    end

    for k=1:rhs
        if typeof(tree.rhs(k))=="cste" & tree.rhs(k).value=="-regexp" then
            set_infos(gettext("Option -regexp not yet handled: will be ignored."),2);
        elseif typeof(tree.rhs(k))=="cste" & tree.rhs(k).value=="-append" then
            set_infos(gettext("Option -append not yet handled: will be ignored."),2);
        end
    end

endfunction
