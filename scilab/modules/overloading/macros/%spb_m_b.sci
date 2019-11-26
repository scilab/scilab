// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
//
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = %spb_m_b(a, b)
    // boolean sparse * boolean
    // r: sparse

    r = bool2s(a) * bool2s(sparse(b))
endfunction
