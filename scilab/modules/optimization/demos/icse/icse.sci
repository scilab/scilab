//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) ????-2010 - DIGITEO - Yann COLLETTE
//
// This file is distributed under the same license as the Scilab package.
//

function [co,u,g,itv,dtv]=icse(u,simu,nap,imp)
    // Computation of the optimal control without scaling of the control
    // and equal weighting of the observations
    // input variables :
    // u(nu)     : initial parameters
    // simu      : string containing the name of the sub program which
    //             describes the problem
    // nap       : maximum number of call to the simulator
    // imp       : debug value during optimization
    // output variables :
    // co        : final cost
    // u(nu)     : final parameters
    // g(nu)     : final gradient
    // itv(nitv) : work area (fortran integers)
    // dtv(ndtv) : work area (fortran double precision)
    // Use the macros icot and icob to extract the state
    df0 = 1;
    nu  = prod(size(u))
    ech = ones(1,nu);
    cof = ones(1,nob*ntob);
    [co,u,g,itv,dtv] = icsegen(u,simu,nap,imp)
endfunction
