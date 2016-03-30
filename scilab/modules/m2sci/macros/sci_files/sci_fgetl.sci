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

function [tree]=sci_fgetl(tree)
    // M2SCI function
    // Conversion function for Matlab fgetl()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    tree.name="mgetl"
    tree.rhs(2)=Cste(1)

    if typeof(tree.lhs(1))=="variable" & tree.lhs(1).name=="ans" then
        out=gettempvar()
        m2sci_insert(Equal(list(out),tree))
        tree=out
    else
        out=tree.lhs(1)
    end

    // meof
    meof_funcall=Funcall("meof",1,list(),list())
    // meof()~=0
    neq=Operation("~=",list(meof_funcall,Cste(0)),list())

    // output=-1
    EQ=Equal(list(out),Cste(-1))

    if typeof(tree)=="variable" then
        m2sci_insert(tlist(["ifthenelse","expression","then","elseifs","else"],neq,list(EQ),list(),list()));
    else
        m2sci_insert(tlist(["ifthenelse","expression","then","elseifs","else"],neq,list(EQ),list(),list()),1);
    end

    if typeof(tree)<>"variable" then
        tree.lhs(1).dims=list(1,Unknown)
        tree.lhs(1).type=Type(Unknown,Real)
    end
endfunction
