// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = gw_genetic_algo()

    demopath = get_absolute_file_path("genetic_algorithms.dem.gateway.sce");

    subdemolist = [_("Genetic algorithms"),             "GAdemo.sce"; ..
    _("Genetic algorithms and Ising problem"), "GAIsing2ddemo.sce"; ..
    _("MultiObjective Genetic Algorithm"),     "MOGAdemo.sce"; ..
    _("Niched Sharing Genetic Algorithm"),     "NSGAdemo.sce"; ..
    _("Niched Sharing Genetic Algorithm II"),  "NSGA2demo.sce"];

    subdemolist(:,2) = demopath + subdemolist(:,2)
endfunction

subdemolist = gw_genetic_algo();
clear gw_genetic_algo;
