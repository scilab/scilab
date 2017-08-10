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

function funcall_tlist=Funcall(name,lhsnb,rhslist,lhslist)
    // Copyright INRIA
    // Create a new function tlist

    rhs=argn(2)

    // Verify input value
    if rhs~=4 then
        error(gettext("Wrong number of inputs."));
    elseif typeof(name)~="string" then
        error(msprintf(gettext("name must be a string instead of a: %s."),typeof(name)));
    elseif typeof(lhsnb)~="constant" then
        error(msprintf(gettext("lhsnb must be a constant instead of a: %s."),typeof(lhsnb)));
    elseif typeof(rhslist)~="list" & rhslist<>[] then
        error(msprintf(gettext("rhslist must be a list instead of a: %s."),typeof(rhslist)));
    elseif typeof(lhslist)~="list" then
        error(msprintf(gettext("lhslist must be a list instead of a: %s."),typeof(lhslist)));
    end

    funcall_tlist=tlist(["funcall","name","lhsnb","rhs","lhs"],name,lhsnb,rhslist,lhslist)
endfunction
