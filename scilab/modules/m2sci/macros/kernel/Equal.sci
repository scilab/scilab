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
        error(msprintf(gettext("expression can not be a: %s."),typeof(expression)))
    end
    if type(endsymbol)<>10 then
        error(msprintf(gettext("endsymbol should be a character string and not a: %s."),typeof(lhslist)));
    end

    eq=tlist(["equal","lhs","expression","endsymbol"],lhslist,expression,endsymbol)

endfunction
