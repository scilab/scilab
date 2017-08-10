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
