// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2020 - ESI Group - Antoine ELIAS
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function c = %sp_a_sp(a, b)
    if isscalar(a) && isscalar(b) then
        c = sparse(full(a) + full(b));
    elseif isscalar(a) then
        c = full(a) + b;
    else
        c = a + full(b);
    end
endfunction
