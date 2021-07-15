// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2012 - 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function dble = convert2double(A)
    // This function converts input parameters to double to avoid to write
    // overloading functions in Scilab

    lhs = argn(1)

    if A.vtype==Double then     // might be ["Double" "Sparse"]
        dble = A
        return
    end

    if only_double then
        dble = A
        if A.vtype==Sparse
            dble.type = Type(Double,A.property)
        else
            dble.type = Type(Double,Unknown)
        end
    else
        if A.vtype==String then
            LHS = Variable("ans", Infer(A.dims,Type(Double,Real)))
            dble = Funcall("asciimat", 1, Rhs_tlist(A), list(LHS))

        elseif A.vtype==Boolean then
            // LHS = Variable("ans",Infer(A.dims,Type(Double,Real)))
            // dble = Funcall("bool2s", 1, Rhs_tlist(A), list(LHS))
            dble = Operation("*", list(A,Cste(1)))
            if is_sparse(A)>0
                dble.type = Type(Sparse,Real)
            else
                dble.type = Type(Double,Real)
            end

        elseif A.vtype==Int then
            LHS = Variable("ans",Infer(A.dims,Type(Double,Real)))
            dble = Funcall("double", 1, Rhs_tlist(A), list(LHS))

        elseif A.vtype==Sparse then
            LHS = Variable("ans",Infer(A.dims,Type(Double,Real)))
            dble = Funcall("full", 1, Rhs_tlist(A), list(LHS))

        elseif A.vtype==Unknown then
            LHS = Variable("ans", Infer(A.dims,Type(Double,Unknown)))
            dble = Funcall("mtlb_double", 1, Rhs_tlist(A), list(LHS))

        else
            msg = gettext("m2sci.convert2double(): Type %s not yet implemented.")
            error(msprintf(msg, string(A.vtype)))
        end
    end
endfunction
