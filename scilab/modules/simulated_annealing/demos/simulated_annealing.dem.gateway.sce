// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
// This file is released into the public domain

demopath = get_absolute_file_path("simulated_annealing.dem.gateway.sce");

subdemolist = ["Simulated Annealing",                   "SAdemo.sce"; ...
	       "Simulated Annealing and Ising problem", "SAIsing2ddemo.sce"];

subdemolist(:,2) = demopath + subdemolist(:,2)
