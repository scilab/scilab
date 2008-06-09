//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//


demopath = get_absolute_file_path("simulation.dem.gateway.sce");

subdemolist = ["n-pendulum"        ,"npend/npend.dem.sce"  ; ..
               "Wheel simulation"  ,"wheel2/wheel.dem.sce" ; ..
               "Bike Simulation"   ,"bike/bike.dem.sce"    ; ..
               "ODE''S"            ,"ode/ode.dem.sce"      ; ..
               "DAE''S"            ,"dae/dae.dem.sce"    ];

subdemolist(:,2) = demopath + subdemolist(:,2);
