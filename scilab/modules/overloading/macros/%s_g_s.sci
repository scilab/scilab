// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
// Copyright (C) 2016 - Scilab Enterprises - Pierre-Aimé AGNEL
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function res=%s_g_s(s1,s2)
    // Logical operand on complex numbers
    res = (s1<>0) | (s2<>0);
endfunction
