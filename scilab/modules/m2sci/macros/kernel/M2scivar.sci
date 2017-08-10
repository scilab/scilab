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

function var=M2scivar(sciname,matname,infer,call_level)
    // Create a new 'm2scivar' tlist

    rhs=argn(2)

    if rhs<3 then
        error(gettext("Wrong number of inputs."))
    end
    if rhs==3 then
        call_level=0
    end

    if typeof(sciname)~="string" then
        error(msprintf(gettext("sciname must be a string instead of a: %s."),typeof(sciname)));
    elseif typeof(matname)~="string" then
        error(msprintf(gettext("matname must be a string instead of a: %s."),typeof(matname)));
    elseif typeof(infer)~="infer" then
        error(msprintf(gettext("infer must be an ''infer'' tlist instead of a: %s."),typeof(infer)));
    end

    var=tlist(["m2scivar","sciname","matname","infer","level"],sciname,matname,infer,call_level)

endfunction
