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

function [r]=%s_n_r(l1,l2)
    //%s_n_r(l1,l2) <=> l1<>l2 l1 constant  l2 rational
    // et l2 une fraction rationnelle
    //!

    r=degree(l2("num"))==0&degree(l2("den"))==0
    if r then r=coeff(l2("num"))./coeff(l2("den"))==l1,end
    r=~r
endfunction
