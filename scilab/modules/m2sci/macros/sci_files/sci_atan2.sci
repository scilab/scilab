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

function [tree]=sci_atan2(tree)
    // M2SCI function
    // Conversion function for Matlab atan2()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // %c_atan and %b_atan are not defined in Scilab
    [Y,X]=getrhs(tree)
    //Y=convert2double(Y)
    //X=convert2double(X)
    tree.rhs=Rhs_tlist(Y,X)

    // Scilab atan() does not work with complex
    if is_complex(Y) then
        tree.rhs(1)=Funcall("real",1,list(Y),list())
    elseif ~is_real(Y) then
        tree.rhs(1)=Funcall("real",1,list(Y),list())
        repl_poss(tree.rhs(1),Y,Y,"is real")
    end
    if is_complex(X) then
        tree.rhs(2)=Funcall("real",1,list(X),list())
    elseif ~is_real(X) then
        tree.rhs(2)=Funcall("real",1,list(X),list())
        repl_poss(tree.rhs(2),X,X,"is real")
    end

    tree.name="atan"

    tree.lhs(1).dims=Y.dims
    tree.lhs(1).type=Type(Double,Real)
endfunction
