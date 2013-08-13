// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("differential_equations.dem.gateway.sce");
    add_demo(gettext("Simulation"), demopath + "differential_equations.dem.gateway.sce");

    subdemolist = [_("n-pendulum")         ,"n_pendulum/n_pendulum.dem.sce" ;
    _("Wheel simulation")   ,"wheel/wheel.dem.gateway.sce"   ;
    _("Bike simulation")    ,"bike/bike.dem.gateway.sce"     ;
    _("ODE''S")             ,"ode/ode.dem.gateway.sce"       ;
    _("DAE''S")             ,"dae/dae.dem.gateway.sce"       ;
    _("Flow simulation")    ,"flow/flow.dem.gateway.sce"     ;
    _("Levitron")           ,"levitron/levitron.dem.sce"     ];


    subdemolist(:,2) = demopath + subdemolist(:,2);
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
