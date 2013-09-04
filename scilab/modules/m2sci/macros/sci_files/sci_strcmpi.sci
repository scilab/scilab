// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
