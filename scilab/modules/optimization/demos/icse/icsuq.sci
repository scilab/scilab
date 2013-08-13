//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
// This file is distributed under the same license as the Scilab package.
//

function [co,u,g,itv,dtv,cof]=icsuq(u,simu,nap,imp,obs,ytob)
    // Computation of the optimal control with weighting of the initial control
    // and geometric weighting of the observation weights.
    // A scaling bring all the componant of the control to 1.
    // The componant initially null will stay null.
    // Only works if the lower bound is positive.
    // Weighting formula :
    // cof(i,j) = 1/2*[(ytob(i,j)-ob(1,j,i))**2+..+(ytob(i,j)-ob(nex,j,i)**2]
    //
    // Syntax
    // [co,u,g,itv,dtv,cof]=icsuq(u,nap,imp,obs,ytob)
    // input variables :
    // u(nu)         : initial parameters
    // simu          : string which contains the name of the sub program
    //                 which describes the problem (second member,
    //                 criterion and initial state)
    // nap           : maximum number of call to the simulator
    // imp           : debug value during optimization
    // obs(nob,ny)   : observation matrix
    // ytob(ny,ntob) : initial value of the state at measure time instants
    //                 obtained by icob after using icse,icsu,icsua or icsuq
    // output variables :
    // co            : final cost
    // u(nu)         : final parameters
    // g(nu)         : final gradient
    // itv(nitv)     : work area (fortran integers)
    // dtv(ndtv)     : work area (fortran double precision)
    // cof(nob,ntob) : weighting coefficients of the cost
    // Use the macros icot and icob to extract the state

    df0 = 1;
    if min(binf) <=0 then
        error("call to icsuq with binf not strictly positive");
    end
    for i=1:nu
        u(1,i)=max([ binf(1,i),min([u(1,i),bsup(1,i)])] );
    end
    ech  = u;
    binf = binf./u;
    bsup = bsup./u;
    u    = ones(1,nu);
    ico  = 2;
    yob  = obs*ytob;
    ob   = don;

    [cof] = fort("icscof",ico,1,"i",ntob,2,"i",nex,3,"i",...
    nob,4,"i",yob,5,"d",ob,6,"d","sort",[1,nob*ntob],7,"d");

    [co,u,g,itv,dtv] = icsegen(u,simu,nap,imp);

    u = ech.*u;
endfunction
