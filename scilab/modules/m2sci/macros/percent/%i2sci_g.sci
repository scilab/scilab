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

function [tree]=%i2sci_g(tree)
    // M2SCI function
    // Conversion function for Matlab insertion in matrices (called by %i2sci())
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree

    from=tree.operands($)
    to=tree.operands(1)
    ind=tree.operands(2)

    newtree=Funcall("mtlb_i",1,Rhs_tlist(to,ind,from),list(to))
    m2sci_insert(Equal(list(to),newtree))
    tree=list()
endfunction


