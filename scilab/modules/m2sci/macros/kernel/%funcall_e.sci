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

function val=%funcall_e(field,m2scitlist)

    val=[]
    if field=="infer" then
        val=m2scitlist.lhs(1).infer
    elseif field=="dims" then
        val=m2scitlist.lhs(1).infer.dims
    elseif field=="type" then
        val=m2scitlist.lhs(1).infer.type
    elseif field=="vtype" then
        val=m2scitlist.lhs(1).infer.type.vtype
    elseif field=="property" then
        val=m2scitlist.lhs(1).infer.type.property
    elseif field=="contents" then
        val=m2scitlist.lhs(1).infer.contents
    else
        error(msprintf(gettext("Extraction of %s from ''%s'' tlist is not yet implemented."),string(field),typeof(m2scitlist)))
    end
endfunction
