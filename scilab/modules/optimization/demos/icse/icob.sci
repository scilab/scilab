//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
// This file is distributed under the same license as the Scilab package.
//

function ytob=icob(dtv)
    // <ytob>=icob(dtv)
    // extraction of the state at measure instant
    // input variables:
    // dtv(ndtv) : double precision workarea obtained by the use of
    //             icse, icsu, icsua or icsuq
    // output variables :
    // ytob(ny,ntob) : value of the state at measure instant

    ytob(ny,ntob) = 0.d0;
    lob = ndtu + ny + ntob + nob*ny + nex*ntob*nob + ...
    2*nu + ntob*nob + ny*(nti+ntf+1+ny+nuc+nuv+1);
    ytob = matrix(dtv(lob+1:lob+ny*ntob),ny,ntob)
endfunction




