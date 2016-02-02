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

function [tree]=sci_meshgrid(tree)
    // M2SCI function
    // Conversion function for Matlab meshgrid()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if rhs==1
        A=getrhs(tree);
        A=convert2double(A);
        tree.rhs=Rhs_tlist(A);
    elseif rhs==2
        [A,B]=getrhs(tree);
        A=convert2double(A);
        B=convert2double(B);
        tree.rhs=Rhs_tlist(A,B);
    end

    if lhs==1
        if tree.rhs(1).dims(1)<>Unknown
            tree.lhs(1).dims=list(tree.rhs(1).dims(1),tree.rhs(1).dims(1));
        else
            tree.lhs(1).dims=list(tree.rhs(1).dims(2),tree.rhs(1).dims(2));
        end
    elseif lhs==2
        if tree.rhs(1).dims(1)<>Unknown
            tree.lhs(1).dims=list(tree.rhs(1).dims(1),tree.rhs(1).dims(1));
        else
            tree.lhs(1).dims=list(tree.rhs(1).dims(2),tree.rhs(1).dims(2));
        end
        if rhs==1
            tree.lhs(2).dims=tree.lhs(1).dims;
        else
            if tree.rhs(2).dims(1)<>Unknown
                tree.lhs(2).dims=list(tree.rhs(2).dims(1),tree.rhs(2).dims(1));
            else
                tree.lhs(2).dims=list(tree.rhs(2).dims(2),tree.rhs(2).dims(2));
            end
        end
    else
        if tree.rhs(1).dims(1)<>Unknown
            tree.lhs(1).dims=list(tree.rhs(1).dims(1),tree.rhs(1).dims(1));
        else
            tree.lhs(1).dims=list(tree.rhs(1).dims(2),tree.rhs(1).dims(2));
        end

        if tree.rhs(2).dims(1)<>Unknown
            tree.lhs(2).dims=list(tree.rhs(2).dims(1),tree.rhs(2).dims(1));
        else
            tree.lhs(2).dims=list(tree.rhs(2).dims(2),tree.rhs(2).dims(2));
        end

        if tree.rhs(3).dims(1)<>Unknown
            tree.lhs(3).dims=list(tree.rhs(3).dims(1),tree.rhs(3).dims(1));
        else
            tree.lhs(3).dims=list(tree.rhs(3).dims(2),tree.rhs(3).dims(2));
        end
    end
endfunction
