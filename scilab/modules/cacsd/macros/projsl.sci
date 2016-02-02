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

function [slp]=projsl(sl,q,m)
    //slp= projected model of sl q*m is the full rank
    //factorization of the projection.
    //!

    slp=syslin(sl(7),m*sl(2)*q,m*sl(3),sl(4)*q,sl(5),m*sl(6))
endfunction
