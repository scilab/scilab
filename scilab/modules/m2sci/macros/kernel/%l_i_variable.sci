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

function var=%l_i_variable(field,inslist,var)
    if field=="dims" then
        var.infer.dims=inslist
    elseif field=="type" then
        var.infer.type=inslist
    elseif field=="contents" then
        var.infer.contents=inslist
    else
        error(gettext("Not yet implemented."))
    end
endfunction
