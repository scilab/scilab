// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
        insert(Equal(list(f),Funcall("gcf",1,list(),list())),1)
    end
    tree.lhs(1).name="ans"
endfunction
