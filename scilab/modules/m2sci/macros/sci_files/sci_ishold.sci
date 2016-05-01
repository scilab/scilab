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

function [tree]=sci_ishold(tree)
    // M2SCI function
    // Conversion function for Matlab ishold()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    gca_funcall=Funcall("gca",1,list(),list())
    get_funcall=Funcall("get",1,Rhs_tlist(gca_funcall,"auto_clear"),list())
    tree=Operation("==",list(get_funcall,Cste("off")),tree.lhs)

    tree.out(1).dims=list(1,1)
    tree.out(1).type=Type(Boolean,Real)

endfunction
