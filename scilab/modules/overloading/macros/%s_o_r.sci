// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function [r]=%s_o_r(l1,l2)
    //%s_o_r(l1,l2) constant==rational
    //!
    r=degree(l2("num"))==0&degree(l2("den"))==0
    if r then r=coeff(l2("num"))./coeff(l2("den"))==l1,end
endfunction
