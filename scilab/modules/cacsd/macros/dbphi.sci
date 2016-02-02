// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA -
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.


function [db,phi]=dbphi(repf,mod)

    [lhs,rhs]=argn(0)
    if rhs==1 then mod="c",end
    phi=phasemag(repf,mod);
    db=20*log(abs(repf))/log(10);
endfunction
