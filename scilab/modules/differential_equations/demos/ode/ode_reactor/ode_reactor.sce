// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - Jakub Kopac <kopac.jakub@gmail.com>
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function nonisothermal_reactor()

    //system of ODEs example
    //nonisothermal plug flow reactor model

    global w dR mc cpc U cp ro //declare global variables for ODE function

    function [F] = odefun (t,y)
        global w dR mc cpc U cp ro
        drH    = [-206300, 55700]; //heat of reaction
        cA     = y(1); //concentration of compound A
        cB     = y(2); //concentration of compound B
        T      = y(3); //temperature of reaction mixture
        Tc     = y(4); //temperature of cooling water
        kv(1)  = 4.25e4*exp(-6775/(T+273.15));   //reaction 1 kinetic factor
        kv(2)  = 6.994e-2*exp(-2471/(T+273.15)); //reaction 2 kinetic factor
        ksi(1) = kv(1)*cA*cB; //reaction 1 rate
        ksi(2) = kv(2)*cB*cB; //reaction 2 rate
        ksi    = ksi(:);
        F(1)   = -ksi(1)/w;            // dC(A)/dz
        F(2)   = (-ksi(1)-2*ksi(2))/w; // dC(B)/dz
        F(3)   = ((-drH)*ksi+4/dR*U*(Tc-T))/w/cp/ro; // dT/dz
        F(4)   = -U*%pi*dR*(Tc-T)/mc/cpc;           // dTc/dz
        F      = F(:); //force column vector
    endfunction

    U   = 646;
    dR  = 0.1; //reactor diameter
    cpc = 4186; //heat capacity of cooling water
    cp  = 2660; //heat capacity
    ro  = 810; //density
    mc  = 0.5; //cooling water mass flow
    V   = 3.22e-4; //volume flow

    w  = V/(%pi*dR*dR/4);
    z  = linspace(0,6); //points to be solution computed
    y0 = [1000, 500, 50, 40]; //initial conditions
    y0 = y0(:); //we need column vector of IC
    solution = ode(y0,0,z,odefun); //solve ODE function

    //extract solution
    cA = solution(1,:);
    cB = solution(2,:);
    T  = solution(3,:);
    Tc = solution(4,:);

    f = scf(1); //create new figure, figure_id=1
    clf    //clear previous graph
    plot(z',[cA; cB]')
    legend("Concentration of compound A","Concentration of compound B"); //make legend for lines
    xlabel("z [m]")  //tag x axis
    ylabel("Concentrations [mol/m3]")  //tag y axis

    g = scf(2); //figure_id=2
    g.figure_position(1) = g.figure_position(1)+f.figure_size(1); // So the windows don't overlay
    clf
    plot(z',[T; Tc]')
    legend("Temperature of reaction mixture","Temperature of cooling water");
    xlabel("z [m]")
    ylabel("Temperature profile [oC]")

endfunction

nonisothermal_reactor();
clear nonisothermal_reactor;
