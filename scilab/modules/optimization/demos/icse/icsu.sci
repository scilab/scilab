//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
// This file is distributed under the same license as the Scilab package.
//

function [co,u,g,itv,dtv]=icsu(u,simu,nap,imp)
    // Computation of the optimal control with weighting of the initial control.
    // A scaling brings all the componant of the initial control to 1.
    // The componant initially null will stay null.
    // Only works if the lower bound is positive.
    // Syntax
    // [co,u,g,itv,dtv] = icsu(u,simu,nap,imp)

    // input variables :
    // u(nu)     : initial parameters
    // simu      : string which contains the name of the sub program
    //             which describes the problem (second member, criterion
    //             and initial state)
    // nap       : maximum number of call to the simulator
    // imp       : debug value during optimization
    // output variables  :
    // co        : final cost
    // u(nu)     : final parameters
    // g(nu)     : final gradient
    // itv(nitv) : work area (fortran integers)
    // dtv(ndtv) : work area (fortran double precision)
    // Use the macros icot and icob to extract the total state
    // or the state at measure time instants of dtv.

    df0 = 1;
    if min(binf) <=0 then
        error("appel de icsu avec binf non strictement positif");
    end
    for i=1:nu
        u(1,i) = max( [binf(1,i),min([u(1,i),bsup(1,i)])] );
    end
    ech  = u;
    binf = binf./u;
    bsup = bsup./u;
    u    = ones(1,nu);
    cof  = ones(1,ntob*nob);
    [co,u,g,itv,dtv] = icsegen(u,simu,nap,imp);
    u = ech.*u;
endfunction
