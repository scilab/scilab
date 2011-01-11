// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [_("Non linear data fitting"), "datafit/datafit.dem.gateway.sce"; ..
               _("Optimisation"),            "optim/optim.dem.gateway.sce"; ..
               _("fminsearch"),              "neldermead/neldermead.dem.gateway.sce"];

if with_module('genetic_algorithms') then
  subdemolist = [subdemolist; ..
                 _("Genetic algorithms"), "../../genetic_algorithms/demos/genetic_algorithms.dem.gateway.sce"];
end

if with_module('simulated_annealing') then
  subdemolist = [subdemolist; ..
                 _("Simulated Annealing"), "../../simulated_annealing/demos/simulated_annealing.dem.gateway.sce"];
end

subdemolist(:,2) = SCI + "/modules/optimization/demos/"+ subdemolist(:,2);
