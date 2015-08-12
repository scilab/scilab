// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function eq=Equal(lhslist,expression,endsymbol)
    // Create a new 'equal' tlist

    rhs=argn(2)

    if rhs<2 then
        error(gettext("Wrong number of inputs."))
    end
    if rhs<3 then
        endsymbol=";"
    end
    if typeof(lhslist)<>"list" then
        error(msprintf(gettext("lhslist should be a list and not a: %s."),typeof(lhslist)));
    end
    if and(typeof(expression)<>["funcall","cste","operation","variable"]) then
        disp(expression)
        error(msprintf(gettext("expression can not be a: %s."),typeof(expression)))
    end
    if type(endsymbol)<>10 then
        error(msprintf(gettext("endsymbol should be a character string and not a: %s."),typeof(lhslist)));
    end

    eq=tlist(["equal","lhs","expression","endsymbol"],lhslist,expression,endsymbol)

endfunction
