// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2015 - Samuel GOUGEON
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function r = %r_o_r(l1, l2)
    r = ((l1.num == l2.num  & l1.den==l2.den)   | ..
    (l1.num == -l2.num & l1.den==-l2.den)       | ..
    (l1.num==0 & l2.num==0)) .. // when simp_mode(%f)
    & (l1.dt==l2.dt)
endfunction
