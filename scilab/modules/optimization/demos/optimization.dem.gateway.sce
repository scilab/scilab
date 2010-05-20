// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

demopath = get_absolute_file_path("optimization.dem.gateway.sce");
subdemolist = ["Non linear data fitting", "datafit/datafit.dem.gateway.sce"; ...
	       "Optimisation",            "optim/optim.dem.gateway.sce"; ...
	       "fminsearch",              "neldermead/neldermead.dem.gateway.sce"];

if with_module('genetic_algorithms') then
  subdemolist = [subdemolist; ...
                 "Genetic algorithms", "../../genetic_algorithms/demos/genetic_algorithms.dem.gateway.sce"];
end 

if with_module('simulated_annealing') then
  subdemolist = [subdemolist; ...
                 "Simulated Annealing", "../../simulated_annealing/demos/simulated_annealing.dem.gateway.sce"];
end

subdemolist(:,2) = demopath + subdemolist(:,2)
