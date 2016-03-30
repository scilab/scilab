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

function [tree]=%i2sci_s(tree)
    // M2SCI function
    // Conversion function for Matlab insertion in scalars (called by %i2sci())
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree

    from=tree.operands($)
    to=tree.operands(1)
    ind=tree.operands(2)

    tree.operands(2)=list(Cste(1),ind)
    if ~is_a_scalar(from) & from.dims(1)<>1 then
        tree.operands($)=Funcall("matrix",1,Rhs_tlist(from,1,Operation("-",list(Cste(1)),list())),list())
    end

    // Data inference
    if typeof(ind)=="cste" then
        tree.out(1).dims=list(1,ind.value)
    else
        tree.out(1).dims=list(1,Unknown)
    end
    tree.out(1).type=to.type
endfunction

