//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Yann COLLETTE
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("icse.dem.gateway.sce");

subdemolist = ["LQV",                               "lqv.sce"; ...
	       "Spaceship landing trajectory",      "navet.sce"; ...
	       "Computation of optimal parameters", "seros.sce";];

subdemolist(:,2) = demopath + subdemolist(:,2)


