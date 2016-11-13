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

function r=%msp_o_msp(l1,l2)
    [ij,v,mn]=spget(l1);l1=sparse(ij,v,mn)
    [ij,v,mn]=spget(l2);l2=sparse(ij,v,mn)
    r=l1==l2
endfunction
