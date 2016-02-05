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

function [tree]=sci_reshape(tree)
    // M2SCI function
    // Conversion function for Matlab reshape()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    // knowndims is a boolean, it's true if all the dimensions of the lhs are known else it's false
    // isemptyrhs is a boolean, it's true if one (or more than one) of the lhs dimensions it's equal to 0 else it's false
    knowndims=%t
    isemptyrhs=%f
    for k=1:rhs
        if k==1 then
            if tree.rhs(k).vtype==String then
                tree.rhs(k)=Funcall("mstr2sci",1,list(tree.rhs(k)),list(Variable("",tree.rhs(k).infer)))
            elseif tree.rhs(k).vtype==Unknown then
                scitree=tree
                scitree.rhs(k)=Funcall("mtlb_double",1,list(tree.rhs(k)),list(Variable("",tree.rhs(k).infer)))
                repl_poss(scitree,tree,tree.rhs(k),gettext("is not a character string matrix."))
                tree=scitree
            end
        end
        if k>=2 then
            for i=1:size(tree.rhs(k).dims)
                if tree.rhs(k).dims(i)==-1
                    knowndims=%f
                end
            end
        end
        if typeof(tree.rhs(k))=="cste" then
            if isempty(tree.rhs(k).value) then
                isemptyrhs=%t
                tree.rhs(k)=Cste(-1)
            end
        end
    end

    set_infos(gettext("WARNING: Matlab reshape() suppresses singleton higher dimension, it is not the case for matrix."),2)
    tree.name="matrix"
    tree.lhs(1).type=tree.rhs(1).type
    if knowndims==%t
        dims=list()
        if size(tree.rhs)==2
            dims=lhsdimsearch(tree.rhs(2))
            tree.lhs(1).dims=dims
        elseif size(tree.rhs)>=2
            if ~isemptyrhs then
                for k=2:size(tree.rhs)
                    if typeof(tree.rhs(k)) == "cste" then
                        dims($+1)=tree.rhs(k).value
                    else
                        dims($+1) = Unknown;
                    end
                end
                tree.lhs(1).dims=dims
            end
        else
            tree.lhs(1).dims=allunknown(tree.rhs(1).dims)
        end
    else
        tree.lhs(1).dims=allunknown(tree.rhs(1).dims)
    end
endfunction

function dims=lhsdimsearch(rhs)
    // LHSDIMSEARCH function searches dimensions size of the lhs
    // dims : a list which contains the size of the lhs
    if typeof(rhs)=="operation" then
        dims=lstcat(rhs.operands(2).value,dims)
        if typeof(rhs.operands(1))=="operation" then
            dims=lhsdimsearch(rhs.operands(1))
        else
            dims=lstcat(rhs.operands(1).value,dims)
        end
    end
endfunction
