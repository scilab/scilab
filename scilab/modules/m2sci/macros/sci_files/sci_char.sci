// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2004-2006 - INRIA - Farid BELAHCENE
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [tree]=sci_char(tree)
    // M2SCI function
    // Conversion function for Matlab char()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if rhs==1 then // char(A)
        A = getrhs(tree)
        if A.vtype==1 | A.vtype==8 | A.vtype==10 then
            tree.lhs(1).dims=tree.rhs(1).dims
            // input is cell
        elseif A.vtype==16 | A.vtype==17 then
            if size(A.contents.data)>0 then
                lhs_dims1=0;
                lhs_dims2=0;
                for i=1:size(A.contents.data)
                    if A.contents.data(i).dims(1)<>-1 & A.contents.data(i).dims(2)<>-1 then
                        lhs_dims1=lhs_dims1+A.contents.data(i).dims(1)
                        lhs_dims2=max(A.contents.data(i).dims(2),lhs_dims2)
                    else
                        lhs_dims1=-1;
                        lhs_dims2=-1;
                    end
                end
                tree.lhs(1).dims(1)=lhs_dims1;
                tree.lhs(1).dims(2)=lhs_dims2;
            end
        elseif A.vtype==Unknown then
            tree.lhs(1).dims=list(-1,-1);
        else
            error(msprintf(gettext("%s is not implemented."),string(A.vtype)))
        end
    elseif rhs>1 //char(t1,t2,t3,...)
        lhs_dims1=0;
        lhs_dims2=0;
        for i=1:rhs
            lhs_dims1=lhs_dims1+tree.rhs(i).dims(1)
            lhs_dims2=max(tree.rhs(i).dims(2),lhs_dims2)
        end
        tree.lhs(1).dims(1)=lhs_dims1;
        tree.lhs(1).dims(2)=lhs_dims2;
    end
    tree.lhs(1).type=Type(String,Real);
endfunction