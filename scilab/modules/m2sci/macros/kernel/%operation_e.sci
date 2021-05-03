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

function val = %operation_e(field, m2scitlist)
if isdef("DEBUG") & DEBUG, disp("%operation_e"), end

    if isfield(m2scitlist, "out") & m2scitlist.out <> list()
        infer = m2scitlist.out(1).infer
    else
        infer = Infer()
    end
    infer0 = Infer()    // default

    if field=="infer" then
        val = infer
    elseif field=="dims" then
        if isfield(infer, "dims"), val = infer.dims, else val = infer0.dims, end
    elseif field=="type" then
        if isfield(infer, "type"), val = infer.type, else val = infer0.type, end
    elseif field=="vtype" then
        if isfield(infer.type, "vtype")
            val = infer.type.vtype,
        else
            val = infer0.type.vtype
        end
    elseif field=="property" then
        if isfield(infer.type, "property")
            val = infer.type.property,
        else
            val = infer0.type.property
        end
    elseif field=="contents" then
        if isfield(infer, "contents")
            val = infer.contents
        else
            val = infer0.contents
        end
    else
        msg = gettext("Extraction of %s from ''%s'' tlist is not yet implemented.")
        error(msprintf(msg, string(field), typeof(m2scitlist)))
    end

endfunction
