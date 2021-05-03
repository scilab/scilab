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

function val = %cste_e(field,m2scitlist)

    val = []
    if field=="infer" then
        f = find(fieldnames(m2scitlist)=="infer")
        if f==[]
            val = Infer()
        else
            val = getfield(f+1, m2scitlist)
        end
    elseif field=="dims" then
        val = m2scitlist.infer.dims
    elseif field=="type" then
        val = m2scitlist.infer.type
    elseif field=="vtype" then
        val = m2scitlist.infer.type.vtype
    elseif field=="property" then
        val = m2scitlist.infer.type.property
    else
        error(msprintf(gettext("Extraction of %s from ''%s'' tlist is not yet implemented."),string(field),typeof(m2scitlist)))
    end
endfunction
