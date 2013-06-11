//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
// This file is distributed under the same license as the Scilab package.
//

function [co,u,g,itv,dtv,cof]=icsua(u,simu,nap,imp)
    // Computation of the optimal control with weighting of the initial control
    // and arithmetic weighting of the observations.
    // A scaling brings all the components of the control to 1.
    // The initially null componants stay null.
    // Only works if the lower bound is positive.
    // Weighting formula :
    // cof(i,j)=nex / (abs(ob(1,j,i)) + ... + abs(ob(nex,j,i)) )
    // Syntax
    // [co,u,g,itv,dtv,cof]=icsua(u,simu,nap,imp)

    // input variables :
    // u(nu)     : initial parameters
    // nap       : maximum number of call to the simulator
    // imp       : debug value during optimization
    // output variables :
    // co            : final cost
    // u(nu)         : final parameters
    // g(nu)         : finale gradient
    // itv(nitv)     : work area (fortran integers)
    // dtv(ndtv)     : work area (fortran double precision)
    // cof(nob,ntob) : weighting coefficient of the cost
    // Use the macros icot and icob to extract the state

    df0 = 1;
    if min(binf) <=0 then
        error("call to icsua with binf not strictly positive");
    end
    for i=1:nu
        u(1,i)=max( [binf(1,i),min([u(1,i),bsup(1,i)])] )
    end
    ech  = u;
    binf = binf./u;
    bsup = bsup./u;
    u    = ones(1,nu);
    ico  = 1;
    yob  = 0.d0*ones(nob,ntob);
    ob   = don;
    [cof] = fort("icscof",ico,1,"i",ntob,2,"i",nex,3,"i",...
    nob,4,"i",yob,5,"d",ob,6,"d","sort",[1,nob*ntob],7,"d");
    [co,u,g,itv,dtv] = icsegen(u,simu,nap,imp);
    u = ech.*u;
endfunction
