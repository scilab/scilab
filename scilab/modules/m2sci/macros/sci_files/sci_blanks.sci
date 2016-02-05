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

function [tree]=sci_blanks(tree)
    // M2SCI function
    // Conversion function for Matlab blanks()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    A=getrhs(tree)
    A=convert2double(A)

    if ~is_a_scalar(A) & typeof(A)<>"variable" then
        n=gettempvar()
        m2sci_insert(Equal(list(n),A))
        B=Operation("ext",list(n,Cste(1)),list())
        repl_poss(B,..
        A,A,gettext("is a scalar."))
    elseif ~is_a_scalar(A) then
        B=Operation("ext",list(A,Cste(1)),list())
        repl_poss(B,..
        A,A,gettext("is a scalar."))
    else
        B=A
    end

    tree.name="part"
    ones_funcall=Funcall("ones",1,list(Cste(1),B),list())
    tree.rhs=list(Cste(" "),ones_funcall)

    if typeof(A)=="cste" then
        tree.lhs(1).dims=list(1,A.value(1))
    else
        tree.lhs(1).dims=list(1,Unknown)
    end
    tree.lhs(1).type=Type(String,Real)
endfunction
