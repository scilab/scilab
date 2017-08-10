// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function operation_tlist=Operation(operator,operands,out)
    // Create a new operation tlist

    rhs=argn(2)

    // Verify input value
    if rhs~=3 then
        error(gettext("Wrong number of inputs."))
    end

    if typeof(operator)~="string" then
        error(msprintf(gettext("operator must be a string instead of a: %s."),typeof(operator)));;
    elseif typeof(operands)~="list" then
        error(msprintf(gettext("operands must be a list instead of a: %s."),typeof(operands)));
    elseif typeof(out)~="list" then
        error(msprintf(gettext("out must be a list instead of a: %s."),typeof(out)));
    end

    operation_tlist=tlist(["operation","operator","operands","out"],operator,operands,out)
endfunction
