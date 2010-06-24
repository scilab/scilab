// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

demopath = get_absolute_file_path("optim.dem.gateway.sce");

subdemolist = ["Rosenbrock", "optim_rosenbrock.sce"; ...
	       "Output",     "optim_output.sce"; ...
	       "Plot",       "optim_plot.sce"; ...
	       "Derivative", "optim_withderivative.sce"];

subdemolist(:,2) = demopath + subdemolist(:,2)


