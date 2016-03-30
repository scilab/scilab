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

function [tree]=sci_hold(tree)
    // M2SCI function
    // Conversion function for Matlab hold()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_hold()

    if rhs==0 then // hold toggle
        tree.name="mtlb_hold"
    else
        opt=tree.rhs(1)
        if typeof(opt)=="cste" then
            if opt.value=="on" then
                // set(gca(),'auto_clear','off')
                gca_funcall=Funcall("gca",1,list(),list())
                tree.name="set"
                tree.rhs=Rhs_tlist(gca_funcall,Cste("auto_clear"),Cste("off"))
            elseif opt.value=="off" then
                // set(gca(),'auto_clear','on')
                gca_funcall=Funcall("gca",1,list(),list())
                tree.name="set"
                tree.rhs=Rhs_tlist(gca_funcall,Cste("auto_clear"),Cste("on"))
            else
                no_equiv(msprintf(gettext("%s option."),opt.value))
            end
        else
            tree.name="mtlb_hold"
        end
    end

endfunction
