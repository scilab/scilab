//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
// This file is distributed under the same license as the Scilab package.
//

function [ytot]=icot(dtv)
    // <ytot>=icot(dtv)
    // extraction of the total state
    // input variables :
    // dtv(ndtv) : double precision workarea obtained by the use of
    //             icse,icsu,icsua or icsuq
    // output variables:
    // ytot(ny,nt) : state of the system

    ytot(ny,nti+ntf) = 0.d0;
    lot  = ndtu + ny + ntob + nob*ny + nex*ntob*nob + ...
    2*nu + ntob*nob + ny*(1+ny+nuc+nuv);
    ytot = matrix(dtv(lot+1:lot+ny*(nti+ntf)),ny,nti+ntf)
endfunction
