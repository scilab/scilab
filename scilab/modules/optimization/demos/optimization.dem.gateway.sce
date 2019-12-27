// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("optimization.dem.gateway.sce");
    _("Optimization and Simulation");  // lets gettext() harvesting it
    add_demo("Optimization and Simulation", demopath+"optimization.dem.gateway.sce");

    subdemolist = [
    _("datafit: Nonlinear fitting"), "datafit/datafit.dem.gateway.sce"
    _("Optimisation"),  "optim/optim.dem.gateway.sce"
    _("fminsearch"),    "neldermead/neldermead.dem.gateway.sce"
    _("ICSE"),          "icse/icse.dem.gateway.sce"
    ];

    subdemolist = [subdemolist; ..
    _("Genetic algorithms"), "genetic/genetic_algorithms.dem.gateway.sce"];

    if with_module("simulated_annealing") then
        subdemolist = [subdemolist; ..
        _("Simulated Annealing"), "annealing/simulated_annealing.dem.gateway.sce"];
    end

    subdemolist(:,2) = demopath + subdemolist(:,2);
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
