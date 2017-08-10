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

function [tree]=sci_linspace(tree)
    // M2SCI function
    // Conversion function for Matlab linspace()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_linspace()

    A=tree.rhs(1)
    B=tree.rhs(2)

    // %c_linspace and %b_linspace are not defined
    if is_complex(A) | is_complex(B) then
        tree.lhs(1).property=Complex
    elseif or(A.vtype==[String,Boolean,Unknown]) | or(B.vtype==[String,Boolean,Unknown]) then
        tree.lhs(1).vtype=Unknown
    else
        tree.lhs(1).vtype=Double
    end

    // y = linspace(A,B)
    if rhs==2 then
        tree.rhs=Rhs_tlist(A,B)
        tree.lhs(1).dims=list(1,100);
        if or(A.vtype==[String,Boolean,Unknown]) | or(B.vtype==[String,Boolean,Unknown]) then
            tree.name = "mtlb_linspace";
        end
    else
        // y = linspace(A,B,n)
        n=tree.rhs(3)
        tree.rhs=Rhs_tlist(A,B,n)
        if typeof(n)=="cste" & n.vtype==Double then
            if isempty(n.value) then
                tree.lhs(1).dims=list(1,1)
            else
                tree.lhs(1).dims=list(1,n.value)
            end
        else
            tree.lhs(1).dims=list(1,Unknown)
        end
        if or(A.vtype==[String,Boolean,Unknown]) | or(B.vtype==[String,Boolean,Unknown]) | or(n.vtype==[String,Boolean,Unknown]) then
            tree.name = "mtlb_linspace";
        end
    end

endfunction
