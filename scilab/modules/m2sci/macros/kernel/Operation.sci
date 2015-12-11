// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
