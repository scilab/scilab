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

function r=%r_o_s(l1,l2)
    // l1==l2 with l1 rational and l2 constant
    //!

    r=degree(l1("num"))==0&degree(l1("den"))==0
    if r then r=coeff(l1("num"))./coeff(l1("den"))==l2,end
endfunction
