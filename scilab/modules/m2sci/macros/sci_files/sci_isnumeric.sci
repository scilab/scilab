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

function [tree]=sci_isnumeric(tree)
    // M2SCI function
    // Conversion function for Matlab isnumeric()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // tf = or(type(A)==[1 5 8])
    rc15=Operation("rc",list(Cste(1),Cste(5)),list())
    rc158=Operation("rc",list(rc15,Cste(8)),list())
    type_funcall=Funcall("type",1,list(tree.rhs(1)),list())
    eq=Operation("==",list(type_funcall,rc158),list())
    tree=Funcall("or",1,Rhs_tlist(eq),tree.lhs)

    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Boolean,Real)

endfunction
