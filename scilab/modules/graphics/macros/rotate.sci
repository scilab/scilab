// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

function [xy]=rotate(xy,teta,orig)
    // effectue une rotation
    //
    // xy   : matrice a deux lignes
    // teta : angle en radian , 0 si omis
    // orig : centre de la rotation, <0;0> si omis
    //!
    [lhs,rhs]=argn(0)
    select rhs
    case 2 then orig=[0;0];
    case 3 then orig=matrix(orig,2,1);
    else error(msprintf(gettext("%s: Wrong number of input argument(s): %d to %d expected."), "scaling", 2, 3));
    end;
    //
    [m,n]=size(xy)
    if m<>2 then
        error(msprintf(gettext("%s: Wrong size for input argument ''%s'': A vector of size %d expected.\n"), "rotate", "xy", 2));
    end
    //
    xy=xy-orig*ones(1,n)
    c=cos(teta),s=sin(teta)
    xy=[c,-s ; s,c]*xy+orig*ones(1,n)
endfunction
