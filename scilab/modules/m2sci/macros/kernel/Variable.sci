// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ???? - INRIA - Scilab
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

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
