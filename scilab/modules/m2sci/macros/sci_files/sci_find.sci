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

function [tree]=sci_find(tree)
    // M2SCI function
    // Conversion function for Matlab find()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_find()

    x = getrhs(tree)

    // Because %c_find is not defined
    x = convert2double(x)
    tree.rhs=Rhs_tlist(x)

    // k = find(x)
    if lhs==1 then
        if is_complex(x) then
            x = Funcall("abs",1,list(x),list(Variable("",x.infer)))
        elseif ~is_real(x) then
            newx = Funcall("abs",1,list(x),list(Variable("",x.infer)))
            repl_poss(newx,x,x,gettext("is Real."));
            x=newx
        end
        tree.rhs=Rhs_tlist(x)

        tree1=Operation("''",list(tree),tree.lhs)
        tree2=Funcall("matrix",1,Rhs_tlist(tree,-1,1),tree.lhs)

        tree.lhs(1).type=Type(Double,Real)

        if x.dims(1)==1 then // Row vector
            tree.lhs(1).dims=list(1,Unknown)
        elseif x.dims(2)==1 then // Column vector
            tree=tree1
            tree.out(1).dims=list(Unknown,1)
            tree.out(1).type=Type(Double,Real)
        elseif not_a_vector(x) then
            tree=tree2
            tree.lhs(1).dims=list(Unknown,Unknown)
        else
            tree.name="mtlb_find"
            tree.lhs(1).dims=list(Unknown,Unknown)
        end
        // [i,j] = find(x)
    elseif lhs==2 then
        if is_complex(x) then
            x = Funcall("abs",1,list(x),list(Variable("",x.infer)))
        elseif ~is_real(x) then
            newx = Funcall("abs",1,list(x),list(Variable("",x.infer)))
            repl_poss(newx,x,x,gettext("is Real."));
            x=newx
        end
        tree.rhs=Rhs_tlist(x)

        tree.lhs(1).type=Type(Double,Real)
        tree.lhs(2).type=Type(Double,Real)
        if x.dims(1)==1 then // Row Vector
            tree.lhs(1).dims=list(1,Unknown)
            tree.lhs(2).dims=list(1,Unknown)
        else
            tree.name="mtlb_find"
            tree.lhs(1).dims=list(Unknown,Unknown)
            tree.lhs(2).dims=list(Unknown,Unknown)
        end
        // [i,j,v] = find(x)
    else
        tree.name="mtlb_find"
        tree.lhs(1).dims=list(Unknown,Unknown)
        tree.lhs(2).dims=list(Unknown,Unknown)
        tree.lhs(3).dims=list(Unknown,Unknown)
        tree.lhs(1).type=Type(Double,Real)
        tree.lhs(2).type=Type(Double,Real)
        tree.lhs(3).type=Type(Unknown,Unknown)
    end

endfunction
