// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA 2016-
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
function [K,X] = lqi(G,Q,R,S)
    //(Linear Quadratic with Integral) controllers are static feedback
    //controllers based upon the augmented states (plant states plus integral
    //of tracking errors) . They stabilize the augmented plants and minimize
    //the quadratic cost functionals associated with step exogenous inputs
    //(step reference inputs and step disturbances).
    //Computes K such that
    // u=-K*[x;xi] which minimize the quadratic cost function
    // J(u) = Integral {z(t)' Q z(t) + u(t)' R u(t) + 2*z(t)' S u(t)} dt (continuous time)
    // J(u) = sum {z(k)' Q z(k) + u(k)'R u(k) + 2*z(k)' S u(k)} (discrete time)
    // where x is the plant states and xi the integrator(s) state(s). xi is
    // the integral of the tracking error e=r-y

    //In the case when not all states are measured, optimal state estimation
    //(Kalman filtering) can be used to realize the LQI controllers.

    // G: state space (syslin) model of the plant
    // Q: square symmetric non-negative matrix. weigths the states contribution
    //   [x',xi']*Q*[x;xi],
    // R: square symmetric non-negative matrix. weigths the control
    //    contribution u'*R*U
    // S: control, state cross contribution  [x',xi']*S*u. The default value
    //    is a zero matrix.

    [ny,nu]=size(G);
    nx=size(G.A,1);
    if or(size(Q)<>nx+ny) then
        error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"lqi",2,nx+ny,nx+ny))
    end

    if or(size(R)<>nu) then
        error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"lqi",3,nu,nu))
    end

    if norm(Q.'-Q,1)>100*%eps*norm(Q,1) then
        error(msprintf(_("%s: Wrong value for input argument #%d: Must be symmetric.\n"),"lqi",2))
    end

    if norm(R.'-R,1)>100*%eps*norm(R,1) then
        error(msprintf(_("%s: Wrong value for input argument #%d: Must be symmetric.\n"),"lqi",3))
    end

    if argn(2)<4 then
        S=zeros(nx+ny,nu);
    elseif or(size(S)<>[nx+ny,nu])  then
        error(msprintf(_("%s: Wrong size for input argument #%d: %d-by-%d matrix expected.\n"),"lqi",4,nx+ny,nu))
    end

    //form the augmented plant  with an integrator.
    [W,Wt]=fullrf([Q S;S' R]);
    C1=Wt(:,1:(nx+ny));
    D12=Wt(:,(nx+ny+1):$);
    if G.dt=="c" then
        Ga=syslin("c",[G.A, zeros(nx,ny); -G.C, zeros(ny,ny)],[G.B; -G.D],C1,D12);
    else
        if G.dt=="d" then
            G.dt=1;
        end

        Ga=syslin(G.dt,[G.A, zeros(nx,ny); -G.C*G.dt, eye(ny,ny)],[G.B; -G.D*G.dt],C1,D12);
    end

    [K,X]=lqr(Ga);
    K=-K
endfunction
