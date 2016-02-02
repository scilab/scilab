// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function R=%ce_t(M)
    D = size(M);
    if size(D,"*") > 2 then
        error(msprintf(_("%s: Wrong size for input argument #%d: Transpose can not be defined.\n"),"%ce_t",1));
    end
    R = makecell([D(2) D(1)], M{:}(:));
endfunction
