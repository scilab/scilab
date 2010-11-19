// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("genetic_algorithms.dem.gateway.sce");

subdemolist = ["Genetic algorithms",                   "GAdemo.sce"; ...
	       "Genetic algorithms and Ising problem", "GAIsing2ddemo.sce"; ...
	       "MultiObjective Genetic Algorithm",     "MOGAdemo.sce"; ...
	       "Niched Sharing Genetic Algorithm",     "NSGAdemo.sce"; ...
	       "Niched Sharing Genetic Algorithm II",  "NSGA2demo.sce" ];

subdemolist(:,2) = demopath + subdemolist(:,2)
clear demopath;