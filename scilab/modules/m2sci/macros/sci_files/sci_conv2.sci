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

function [tree]=sci_conv2(tree)
    // M2SCI function
    // Conversion function for Matlab conv2()
    // Input: tree = Matlab funcall tree
    // Ouput: tree = Scilab equivalent for tree

    if rhs==2 then // conv2(A,B)
        [A,B]=getrhs(tree)
        A=convert2double(A)
        B=convert2double(B)
        if is_real(A)&is_real(B) then
            tree.lhs(1).type=Type(Double,Real)
        elseif is_complex(A)|is_complex(B) then
            tree.lhs(1).type=Type(Double,Complex)
        else
            tree.lhs(1).type=Type(Double,Unknown)
        end
    elseif rhs==3 then
        [A,B,C]=getrhs(tree)
        if C.type==String then // conv(A,B,frame)
            A=convert2double(A)
            B=convert2double(B)
            if is_real(A)&is_real(B) then
                tree.lhs(1).type=Type(Double,Real)
            elseif  is_complex(A)|is_complex(B) then
                tree.lhs(1).type=Type(Double,Complex)
            else
                tree.lhs(1).type=Type(Double,Unknown)
            end
        else // conv(u,v,B)
            A=convert2double(A)
            B=convert2double(B)
            C=convert2double(C)
            if is_real(A)&is_real(B)&is_real(C) then
                tree.lhs(1).type=Type(Double,Real)
            elseif is_complex(A)|is_complex(B)|is_complex(C) then
                tree.lhs(1).type=Type(Double,Complex)
            else
                tree.lhs(1).type=Type(Double,Unknown)
            end
        end
    elseif rhs==4 then  // conv(u,v,B, frame)
        [A,B,C,D]=getrhs(tree)
        A=convert2double(A)
        B=convert2double(B)
        C=convert2double(C)
        if is_real(A)&is_real(B)&is_real(C) then
            tree.lhs(1).type=Type(Double,Real)
        elseif is_complex(A)|is_complex(B)|is_complex(C) then
            tree.lhs(1).type=Type(Double,Complex)
        else
            tree.lhs(1).type=Type(Double,Unknown)
        end
    end
    tree.lhs(1).dims=list(Unknown,Unknown);
endfunction
