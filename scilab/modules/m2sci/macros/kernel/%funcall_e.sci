// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2020 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function val = %funcall_e(field, m2scitlist)

    if isfield(m2scitlist, "lhs") then
        LHS = m2scitlist.lhs
        if LHS==list()
            LHS = list(Infer())
        end
    else
        LHS = list(Infer())
    end
    if field=="lhs" then
        val = LSH
    elseif field=="infer" then
        val = LHS(1)
    elseif field=="dims" then
        val = LHS(1).dims
    elseif field=="type" then
        val = LHS(1).type
    elseif field=="vtype" then
        val = LHS(1).type.vtype
    elseif field=="property" then
        val = LHS(1).type.property
    elseif field=="contents" then
        val = LHS(1).contents
    else
        msg = gettext("Extraction of ''%s'' from ''%s'' tlist is not yet implemented.\n")
        error(msprintf(msg, string(field), typeof(m2scitlist)))
    end
endfunction
