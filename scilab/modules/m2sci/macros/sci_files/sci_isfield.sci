// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_isfield(tree)
    // M2SCI function
    // Conversion function for Matlab isfield()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    st=tree.rhs(1)
    f=tree.rhs(2)
    if and(st.vtype<>[Struct,Unknown]) then
        tree=Cste(%F)
    elseif st.vtype==Unknown then
        tree.name="mtlb_tree"
        tree.lhs(1).dims=list(1,1)
        tree.lhs(1).type=Type(Boolean,Real)
    else
        // allf=getfield(1,st);tf=or(allf(3:$)==f);
        allf=gettempvar()
        insert(Equal(list(allf),Funcall("getfield",1,Rhs_tlist(1,st),list())))

        imp=Operation(":",list(Cste(3),Cste("$")),list())
        ext=Operation("ext",list(allf,imp),list())
        eq=Operation("==",list(ext,f),list())

        tree=Funcall("or",1,Rhs_tlist(eq),tree.lhs)
        tree.lhs(1).dims=list(1,1)
        tree.lhs(1).type=Type(Boolean,Real)
    end
endfunction
