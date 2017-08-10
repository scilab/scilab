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
        m2sci_insert(Equal(list(allf),Funcall("getfield",1,Rhs_tlist(1,st),list())))

        imp=Operation(":",list(Cste(3),Cste("$")),list())
        ext=Operation("ext",list(allf,imp),list())
        eq=Operation("==",list(ext,f),list())

        tree=Funcall("or",1,Rhs_tlist(eq),tree.lhs)
        tree.lhs(1).dims=list(1,1)
        tree.lhs(1).type=Type(Boolean,Real)
    end
endfunction
