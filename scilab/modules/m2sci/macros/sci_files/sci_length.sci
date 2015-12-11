// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_length(tree)
    // M2SCI function
    // Conversion function for Matlab length()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    X = getrhs(tree)
    // Conversion to double is made to have the same results for strings
    if or(X.vtype==[String,Unknown]) then
        X = convert2double(X)
        tree.rhs=list(X)
    end

    if is_a_vector(X) then
        tree=Funcall("size",1,Rhs_tlist(X,"*"),tree.lhs)
    else
        tree.name="size"
        tree=Funcall("max",1,Rhs_tlist(tree),tree.lhs)
    end
    tree.lhs(1).dims=list(1,1)
    tree.lhs(1).type=Type(Double,Real)

endfunction
