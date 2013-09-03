// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt

function r=%msp_o_msp(l1,l2)
    [ij,v,mn]=spget(l1);l1=sparse(ij,v,mn)
    [ij,v,mn]=spget(l2);l2=sparse(ij,v,mn)
    r=l1==l2
endfunction
