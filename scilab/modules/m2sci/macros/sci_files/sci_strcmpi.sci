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

function [tree]=sci_strcmpi(tree)
    // M2SCI function
    // Conversion function for Matlab strcmpi()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_strcmpi()

    [A,B] = getrhs(tree)

    if A.vtype==String & B.vtype==String then
        A = Funcall("convstr",1,Rhs_tlist(A),list())
        B = Funcall("convstr",1,Rhs_tlist(B),list())
        tree = Operation("==",list(A,B),tree.lhs)
        tree.out(1).dims=list(1,1)
        tree.out(1).type=Type(Boolean,Real)
    elseif A.vtype<>Unknown & B.vtype<>Unknown then
        // At least one argument is not a character string matrix in call of strcmpi() so result is set to 0
        tree = Cste(%F)
    else
        tree.name="mtlb_strcmpi"
        tree.lhs(1).dims=list(1,1)
        tree.lhs(1).type=Type(Boolean,Real)
    end
endfunction
