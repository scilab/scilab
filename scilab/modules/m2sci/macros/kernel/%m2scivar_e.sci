// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2002-2004 - INRIA - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function val=%m2scivar_e(field,m2scitlist)

    val=[]
    if field=="infer" then
        val=m2scitlist.infer
    elseif field=="dims" then
        val=m2scitlist.infer.dims
    elseif field=="type" then
        val=m2scitlist.infer.type
    elseif field=="vtype" then
        val=m2scitlist.infer.type.vtype
    elseif field=="property" then
        val=m2scitlist.infer.type.property
    elseif field=="contents" then
        val=m2scitlist.infer.contents
    else
        error(msprintf(gettext("Extraction of %s from ''%s'' tlist is not yet implemented."),string(field),typeof(m2scitlist)))
    end
endfunction
