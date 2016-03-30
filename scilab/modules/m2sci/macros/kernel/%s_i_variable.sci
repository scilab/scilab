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

function var=%s_i_variable(field,cste,var)

    if field=="vtype" then
        var.infer.type.vtype=cste
    elseif field=="property" then
        var.infer.type.property=cste
    elseif field=="contents" then
        var.infer.contents=cste
    else
        error(gettext("Not yet implemented."))
    end
endfunction
