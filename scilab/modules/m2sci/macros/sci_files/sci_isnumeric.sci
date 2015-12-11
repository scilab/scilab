// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
