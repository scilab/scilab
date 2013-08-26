// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_isvector(tree)
    // M2SCI function
    // Conversion function for Matlab isvector()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if or(tree.rhs(1).vtype==[String,Unknown]) then
        tree.rhs(1)=convert2double(tree.rhs(1))
    end

    rowsize=Funcall("size",1,Rhs_tlist(tree.rhs(1),1),list())
    colsize=Funcall("size",1,Rhs_tlist(tree.rhs(1),2),list())

    rowsizeeq=Operation("==",list(rowsize,Cste(1)),list())
    colsizeeq=Operation("==",list(colsize,Cste(1)),list())

    tree=Operation("|",list(rowsizeeq,colsizeeq),tree.lhs)

    tree.out(1).dims=list(1,1)
    tree.out(1).type=Type(Boolean,Real)

endfunction
