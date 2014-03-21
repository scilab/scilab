//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO
//
// This file is distributed under the same license as the Scilab package.
//

function [co,u,g,itv,dtv]=icsegen(u,simu,nap,imp,ech,cof)
    // Computation of the optimal control with scaling of the control and
    // weighting of the observations
    // Syntax
    // [co,u,g,itv,dtv]=icsegen(u,simu,nap,imp,ech,cof)
    //
    // input variables :
    // u(nu)     : initial parameters
    // simu      : string which contains the name of the sub program which
    //             describes the problem (second member, criterion and initial state)
    // nap       : maximum number of calls to the simulator
    // imp       : debug value during optimization
    // ech(1,nu) : scaling coeff of the control
    // cof(1,ntob*nob) : weighting coeff of the observations
    // output variables :
    // co        : final cost
    // u(nu)     : final parameters
    // g(nu)     : final gradient
    // itv(nitv) : work area (fortran integers)
    // dtv(ndtv) : work area (fortran double precision)
    // Use the icot and icob macro to extract the state

    if nu<large then
        alg="qn";
    else
        alg="gc";
    end

    nitv  = nitu+ny+ntob;

    ldtvt = ny*(nob+nuc+nuv+nti+ntf+ny+3)+ntob*(nob*nex+nob+1)+2*nu+ndtu;
    ndtv1 = ldtvt+ny*(ny+7);
    nui   = iu(1)*nuc+iu(2)*nuv*(nti+ntf+1);
    ndtv2 = ldtvt+ny*(2*ntob+nu+2*ny+nuc+nuv+4)+max(nuc+nuv,nui)+nob*ntob+nob;
    ndtv  = max(ndtv1,ndtv2);

    itv         = itu;
    itv($:nitv) = 0;
    dtv         = [dtu,y0,tob,matrix(obs,1,ny*nob),don,ech,cof,b,fy(1,:),fu(1,:)];
    dtv($:ndtv) = 0;

    [co,u,g,itv,dtv] = optim(simu,"b",binf,bsup, u, alg, df0, ...
    "ar",nap, "ti",itv,"td",dtv,"si","sd");
endfunction
