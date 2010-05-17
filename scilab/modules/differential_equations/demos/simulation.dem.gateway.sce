//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("simulation.dem.gateway.sce");

subdemolist = ["n-pendulum"         ,"n_pendulum/n_pendulum.dem.sce" ;
               "Wheel simulation"   ,"wheel/wheel.dem.gateway.sce"   ;
               "Bike simulation"    ,"bike/bike.dem.gateway.sce"     ;
               "ODE''S"             ,"ode/ode.dem.gateway.sce"       ;
               "DAE''S"             ,"dae/dae.dem.gateway.sce"       ];

if ~ usecanvas() then
	subdemolist = [ subdemolist                               ; ..
		"Flow simulation"   ,"flow/flow.dem.gateway.sce" ; ..
		"Levitron"          ,"levitron/levitron.dem.sce" ];
end

subdemolist(:,2) = demopath + subdemolist(:,2);
