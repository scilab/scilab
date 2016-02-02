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

function [tree]=sci_clf(tree)
    // M2SCI function
    // Conversion function for Matlab clf()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if rhs<1 then
        set_infos(gettext("All children will be deleted, no HandleVisibility property in Scilab graphics."),2);
    end
    if rhs==1 then
        set_infos(gettext("All properties will be reset."),2);
    end

    // f=gcf() inserted if output value stored in a variable
    if tree.lhs(1).name<>"ans" then
        f=tree.lhs(1)
        m2sci_insert(Equal(list(f),Funcall("gcf",1,list(),list())),1)
    end
    tree.lhs(1).name="ans"
endfunction
