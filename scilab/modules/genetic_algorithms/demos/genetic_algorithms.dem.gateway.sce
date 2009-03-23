//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("genetic_algorithms.dem.gateway.sce");
subdemolist = ["Genetic algorithms",                   "GAdemo.sce"; ...
	       "Genetic algorithms and Ising problem", "GAIsing2ddemo.sce"; ...
	       "MultiObjective Genetic Algorithm",     "MOGAdemo.sce"; ...
	       "Niched Sharing Genetic Algorithm",     "NSGAdemo.sce"; ...
	       "Niched Sharing Genetic Algorithm II",  "NSGA2demo.sce" ];
subdemolist(:,2) = demopath + subdemolist(:,2)
