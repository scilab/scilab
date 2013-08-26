// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [r]=%r_n_s(l1,l2)
    // l1<>l2     rational<>constant

    r=degree(l1("num"))==0&degree(l1("den"))==0
    if r then r=coeff(l1("num"))./coeff(l1("den"))==l2,end
    r=~r
endfunction
