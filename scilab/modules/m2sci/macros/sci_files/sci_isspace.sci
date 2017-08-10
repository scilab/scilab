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

function [tree]=sci_isspace(tree)
    // M2SCI function
    // Conversion function for Matlab isspace()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_isspace()

    A = getrhs(tree)

    if A.vtype==String then
        A = Funcall("asciimat",1,Rhs_tlist(A),list(Variable("",A.infer)))
        tree = Operation("==",list(A,Cste(32)),tree.lhs)
        tree.out(1).dims=A.dims
        tree.out(1).type=Type(Boolean,Real)
    elseif or(A.vtype==[Double,Boolean]) then
        tree.name="zeros"
        tree.lhs(1).dims=A.dims
        tree.lhs(1).type=Type(Boolean,Real)
    else
        tree.name="mtlb_isspace"
        tree.lhs(1).dims=A.dims
        tree.lhs(1).type=Type(Boolean,Real)
    end

endfunction
