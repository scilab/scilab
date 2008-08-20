//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("simulation.dem.gateway.sce");

subdemolist = ["Simple ODE"                   ,"ode1.dem.sce"       ; ..
               "Chemical process (Stiff)"     ,"ode2.dem.sce"       ; ..
               "Chemical process (Implicit)"  ,"ode3.dem.sce"       ; ..
               "Lorenz equation"              ,"ode4.dem.sce"       ; ..
               "ODE 1D vector field"          ,"odenew1_1.dem.sce"  ; ..
               "Van der Pol vector field"     ,"vanderpol.dem.sce"  ; ..
               "Lotka-Volterra vector field"  ,"lotka.dem.sce" ];

subdemolist(:,2) = demopath + subdemolist(:,2);
