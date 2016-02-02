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

function var=Variable(name,infer)
    // Create a new variable tlist

    rhs=argn(2)

    if rhs<>2 then
        error(gettext("Wrong number of inputs."))
    end

    if typeof(name)~="string" then
        error(msprintf(gettext("name must be a string instead of a: %s."),typeof(name)))
    elseif typeof(infer)~="infer" then
        error(msprintf(gettext("infer must be an ''infer'' tlist instead of a: %s."),typeof(infer)))
    end

    var=tlist(["variable","name","infer"],name,infer)

endfunction
