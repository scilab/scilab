// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("ode.dem.gateway.sce");

subdemolist = ["Simple ODE"                   ,"ode_simple/ode_simple.dem.sce"       ; ..
"Chemical process (Stiff)"     ,"ode_chstiff/ode_chstiff.dem.sce"     ; ..
"Chemical process (Implicit)"  ,"ode_chimpl/ode_chimpl.dem.sce"       ; ..
"Lorenz equation"              ,"ode_lorenz/ode_lorenz.dem.sce"     ; ..
"ODE 1D vector field"          ,"ode_1dvect/ode_1dvect.dem.sce"       ; ..
"Van der Pol vector field"     ,"ode_vanderpol/ode_vanderpol.dem.sce" ; ..
"Nonisothermal plug flow reactor model"     ,"ode_reactor/ode_reactor.sce" ; ..
"Lotka-Volterra vector field"  ,"ode_lotka/ode_lotka.dem.sce"         ];

subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;