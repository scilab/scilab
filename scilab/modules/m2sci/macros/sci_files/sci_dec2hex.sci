// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [tree]=sci_dec2hex(tree)
    // M2SCI function
    // Conversion function for Matlab dec2hex()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // str=dec2hex(D)
    if rhs==1 then
        D=getrhs(tree)
        D=convert2double(D)

        if ~is_real(D) then
            newD=Funcall("real",1,list(D),list(D))
            repl_poss(newD,D,D,gettext("is Real."));
            D=newD
        end
        tree.rhs=list(D)

        if is_empty(D) then
            tree=Cste("")
            return
        elseif not_empty(D) then
            tree=Funcall("matrix",1,list(tree,Cste(-1),Cste(1)),tree.lhs)
            if is_a_scalar(D) then
                tree.lhs(1).dims=list(1,Unknown)
            elseif is_a_vector(D) & ( D.dims(1)==1 | D.dims(2)>1 ) then // Row vector
                tree.lhs(1).dims=list(D.dims(2),Unknown)
            elseif is_a_vector(D) & ( D.dims(1)>1 | D.dims(2)==1 ) then
                tree.lhs(1).dims=list(D.dims(1),Unknown)
            else
                tree.lhs(1).dims=list(Unknown,Unknown)
            end
        else
            tree.name="mtlb_dec2hex"
            tree.lhs(1).dims=list(Unknown,Unknown)
        end

        // str=dec2hex(D,N)
    else
        [D,N]=getrhs(tree)
        D=convert2double(D)
        N=convert2double(N)
        tree.rhs=list(D,N)

        tree.name="mtlb_dec2hex"

        if is_empty(D) then
            tree=Cste("")
            return
        elseif not_empty(D) & typeof(N)=="cste" then
            tree.lhs(1).dims=list(Unknown,N.value)
        end
    end
    tree.lhs(1).type=Type(String,Real)
endfunction
