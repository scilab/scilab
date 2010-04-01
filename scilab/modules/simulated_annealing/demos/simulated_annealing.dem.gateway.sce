//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("simulated_annealing.dem.gateway.sce");

subdemolist = ["Simulated Annealing",                   "SAdemo.sce"; ...
	       "Simulated Annealing and Ising problem", "SAIsing2ddemo.sce"];

subdemolist(:,2) = demopath + subdemolist(:,2)
