// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_findstr(tree)
    // M2SCI function
    // Conversion function for Matlab findstr()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_findstr()

    set_infos(gettext("No equivalent for findstr() in Scilab so mtlb_findstr() is called."),0);

    tree.name="mtlb_findstr"

    tree.lhs(1).dims=list(Unknown,Unknown)
    tree.lhs(1).type=Type(Double,Real)
endfunction
