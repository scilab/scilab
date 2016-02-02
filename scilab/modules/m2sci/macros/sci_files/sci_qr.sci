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

function [tree]=sci_qr(tree)
    // M2SCI function
    // Conversion function for Matlab qr()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree
    // Emulation function: mtlb_qr()

    // qr(A)
    if rhs==1 then
        A = getrhs(tree)
        A = convert2double(A)
        tree.rhs=Rhs_tlist(A)
        if is_real(A) then
            prop=Real
        else
            prop=Unknown
        end
        // X = qr(A)
        if lhs==1 then
            if tree.lhs(1).name=="ans" then
                no_equiv(expression2code(tree))
            else
                no_equiv(tree.lhs(1).name+" = "+expression2code(tree))
            end
            // [Q,R] = qr(A)
        elseif lhs==2 then
            tree.lhs(1).dims=list(A.dims(1),A.dims(1))
            tree.lhs(1).type=A.type
            tree.lhs(2).dims=A.dims
            tree.lhs(2).type=Type(A.vtype,prop)
            // [Q,R,E] = qr(A)
        else
            tree.lhs(1).dims=list(A.dims(1),A.dims(1))
            tree.lhs(1).type=A.type
            tree.lhs(2).dims=A.dims
            tree.lhs(2).type=Type(A.vtype,prop)
            tree.lhs(3).dims=list(A.dims(2),A.dims(2))
            tree.lhs(3).type=Type(A.vtype,prop)
        end
        // qr(A,0)
    else
        [A,B] = getrhs(tree)
        A = convert2double(A)
        tree.rhs=Rhs_tlist(A,B)
        if is_real(A) then
            prop=Real
        else
            prop=Unknown
        end
        // R = qr(A,0)
        if lhs==1 then
            if tree.lhs(1).name=="ans" then
                no_equiv(expression2code(tree))
            else
                no_equiv(tree.lhs(1).name+" = "+expression2code(tree))
            end
            // [Q,R] = qr(A,0)
        elseif lhs==2 then
            tree.rhs=Rhs_tlist(A,"e")
            tree.lhs(1).dims=list(Unknown,Unknown)
            tree.lhs(1).type=A.type
            tree.lhs(2).dims=list(Unknown,Unknown)
            tree.lhs(2).type=Type(A.vtype,prop)
            // [Q,R,E] = qr(A,0)
        else
            set_infos(gettext("Scilab and Matlab qr() do not give same results for this case: mtlb_qr() is used."),0)
            tree.name="mtlb_qr"
            tree.lhs(1).dims=list(Unknown,Unknown)
            tree.lhs(1).type=Type(A.vtype,prop)
            tree.lhs(2).dims=list(Unknown,Unknown)
            tree.lhs(2).type=Type(A.vtype,prop)
            tree.lhs(3).dims=list(Unknown,Unknown)
            tree.lhs(3).type=Type(A.vtype,prop)
        end
    end

endfunction
