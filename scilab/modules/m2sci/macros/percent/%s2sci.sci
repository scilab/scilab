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

function [tree]=%s2sci(tree)
    // M2SCI function
    // Conversion function for Matlab subtraction
    // Input: tree = Matlab operation tree
    // Output: tree = Scilab equivalent for tree
    // Emulation function: mtlb_s()

    // In Matlab only two matrices with the same size can be added unless one is a scalar
    // So empty matrix can only be added to a scalar or an onther empty matrix
    // For example: [1,2,3]-[] is not possible
    // An other important difference with Scilab is that:
    //  - Matlab gives : [1]-[]=[]
    //  - Scilab gives : [1]-[]=[1]

    // Overloading functions in $SCI/modules/compatibility_functions/macros/:
    // - %b_s_s.sci
    // - %s_s_b.sci

    // Binary operator: A-B
    if size(tree.operands)==2 then
        [A,B]=getoperands(tree)

        // Matlab and Scilab subtraction do not match for Strings
        if or(A.vtype==[String,Unknown]) then
            A=convert2double(A)
        end
        if or(B.vtype==[String,Unknown]) then
            B=convert2double(B)
        end

        // %b_s_b is not defined in Scilab
        if A.vtype==Boolean & B.vtype==Boolean
            A=convert2double(A)
        end

        tree.operands=list(A,B)

        // Type inference
        if (is_complex(A) & is_real(B)) | (is_complex(B) & is_real(A)) then
            tree.out(1).type=Type(Double,Complex)
        elseif is_real(A) & is_real(B) then
            tree.out(1).type=Type(Double,Real)
        else
            tree.out(1).type=Type(Double,Unknown)
        end

        // When both operands are not [] Scilab and Matlab give the same results
        if not_empty(A) & not_empty(B) then
            if is_a_scalar(A) then
                tree.out(1).dims=B.dims
            else
                tree.out(1).dims=A.dims
            end
            // If at least one operand is [] then Matlab result is [] but not Scilab one
        elseif is_empty(A) | is_empty(B) then
            set_infos(msprintf(gettext("At least one operand of %s is an empty matrix, Scilab equivalent is []."),expression2code(tree)),0)
            tree=Cste([])
        else
            tree.out(1).dims=allunknown(A.dims);
            tree=Funcall("mtlb_s",1,list(A,B),tree.out);
        end
    else
        A=getoperands(tree)
        //A=convert2double(A)
        tree.operands=list(A)
        tree.out(1).dims=A.dims
        tree.out(1).type=A.type
    end
endfunction
