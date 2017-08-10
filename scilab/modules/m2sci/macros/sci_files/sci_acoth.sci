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

function tree=sci_acoth(tree)
    // M2SCI function
    // Conversion function for Matlab acoth()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    X=getrhs(tree)
    X=convert2double(X)

    set_infos(msprintf(gettext("If %s is outside [-1,1]\n   complex part of output of %s will be the opposite of Matlab one."), strcat(expression2code(X), ""), strcat(expression2code(tree), "")),2)

    tree.name="atanh"
    Op=Operation("./",list(Cste(1),X),list())
    tree.rhs=Rhs_tlist(Op)

    tree.lhs(1).dims=X.dims
    // Property unknown because result can be complex for real input
    tree.lhs(1).type=Type(X.vtype,Unknown)
endfunction
