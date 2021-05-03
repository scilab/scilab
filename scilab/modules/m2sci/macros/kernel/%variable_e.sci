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

function val = %variable_e(field,m2scitlist)
if isdef("DEBUG") & DEBUG then disp("%variable_e"), end

    val=[]
    if isfield(m2scitlist, "infer") then
        infer = m2scitlist.infer
    else
        infer = Infer()
    end
    if field=="infer" then
        val = infer
    elseif field=="dims" then
        val = infer.dims
    elseif field=="type" then
        val = infer.type
    elseif field=="vtype" then
        val = infer.type.vtype
    elseif field=="property" then
        val = infer.type.property
    elseif field=="contents" then
        val = infer.contents
    else
        error(msprintf(gettext("Extraction of ''%s'' from ''%s'' tlist is not yet implemented."),string(field),typeof(m2scitlist)))
    end

endfunction
