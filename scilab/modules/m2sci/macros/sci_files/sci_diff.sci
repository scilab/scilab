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

function [tree]=sci_diff(tree)
    // M2SCI function
    // Conversion function for Matlab diff()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_diff()

    // Y = diff(X)
    if rhs==1 then
        X=getrhs(tree)
        // %c_diff and %b_diff are not defined in Scilab
        X=convert2double(X)
        tree.rhs=Rhs_tlist(X)

        dim = first_non_singleton(X)

        if dim==-1 then
            // All output dims are unknown
            tree.lhs(1).dims=allunknown(X.dims)
            // First non singleton dimension will be computed at execution
            tree.rhs=Rhs_tlist(X,1,Funcall("firstnonsingleton",1,list(X),list()))
        else
            tree.lhs(1).dims=X.dims
            if dim==0 then
                tree.rhs=Rhs_tlist(X)
                if is_a_scalar(X) then // Special case for scalar input, output is []
                    tree.lhs(1).dims = list(0,0);
                end
                if is_a_vector(X) then // Special case for vector input (all dimensions known and one is greater than 1)
                    for n=1:size(X.dims)
                        if tree.lhs(1).dims(n)<>1 then
                            tree.lhs(1).dims(n)=tree.lhs(1).dims(n)-1;
                        end
                    end
                end
            else
                tree.rhs=Rhs_tlist(X,1,dim)
                tree.lhs(1).dims(dim)=max(X.dims(dim)-1,-1)
            end
        end
        tree.lhs(1).type=Type(Double,Unknown)
        // Y = diff(X,n)
    elseif rhs==2 then
        [X,n] = getrhs(tree);
        X = convert2double(X)
        n = convert2double(n)
        tree.rhs=Rhs_tlist(X,n)

        if ~is_a_vector(X) then
            tree.name="mtlb_diff"
        end
        tree.lhs(1).dims=allunknown(X.dims)
        tree.lhs(1).type=Type(Double,Unknown)

        // Y = diff(X,n,dim)
    else
        [X,n,dim] = getrhs(tree);
        X = convert2double(X)
        n = convert2double(n)
        dim = convert2double(dim)
        tree.rhs=Rhs_tlist(X,n,dim)

        tree.lhs(1).dims=allunknown(X.dims)
        tree.lhs(1).type=Type(Double,Unknown)
    end

endfunction
