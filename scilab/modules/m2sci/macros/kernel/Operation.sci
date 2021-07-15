// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function operation_tlist = Operation(operator,operands,out)
    // Create a new operation tlist
    rhs = argn(2)
    fname = "m2sci.Operation"

    // Verify input value
    if rhs < 2 | rhs > 3 then
        msg = gettext("%s: Wrong number of input arguments: %d or %d expected.\n");
        error(msprintf(msg, fname, 2, 3))
    end
    if rhs==2 then
        out = list()
    end

    if typeof(operator)~="string" then
        msg = gettext("%s: operator as Argument #%d must be a string instead of a ''%s''.\n")
        error(msprintf(msg, fname, 1, typeof(operator)));

    elseif typeof(operands)~="list" then
        msg = gettext("%s: operands as Argument #%d must be a list instead of a ''%s''.\n");
        error(msprintf(msg, fname, 2, typeof(operands)));

    elseif typeof(out)~="list" then
        msg = gettext("%s: out as Argument #%d must be a list instead of a ''%s''.\n");
        error(msprintf(msg, fname, 3, typeof(out)));
    end

    // Setting the operation's list
    // ----------------------------
    operation_tlist = tlist(["operation","operator","operands","out"], operator, operands, out)
    // Setting some default properties
    // -------------------------------
    // Comparisons => vtype = Boolean, property = Boolean|Sparse|Unknown
    if or(operator==["<" "<=" "==" "~=" ">=" ">"]) // & ..
            // (or(operands(1).vtype==[Sparse, Boolean]) | ..
            // or(operands(2).vtype==[Sparse, Boolean])) then
        [L, R] = (operands(1), operands(2));
        if is_sparse(L)>0 | is_sparse(R)>0
            operation_tlist.type = Type(Boolean, Sparse)
        else
            [tL, tR] = (L.type, R.type)
            if tL.vtype==Unknown | tR.vtype==Unknown |..
               (tL.vtype==Boolean & tL.property==Unknown) | ..
               (tR.vtype==Boolean & tR.property==Unknown)
                operation_tlist.type = Type(Boolean,Unknown)
            else
                operation_tlist.type = Type(Boolean,Boolean)
            end
        end
    end

endfunction
