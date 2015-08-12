// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_blanks(tree)
    // M2SCI function
    // Conversion function for Matlab blanks()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    A=getrhs(tree)
    A=convert2double(A)

    if ~is_a_scalar(A) & typeof(A)<>"variable" then
        n=gettempvar()
        insert(Equal(list(n),A))
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
