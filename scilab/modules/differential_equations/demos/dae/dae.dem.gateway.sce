// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("dae.dem.gateway.sce");

subdemolist = ["Spherical pendulum" ,"dae1/pend3d1.dem.sce" ;
"Sliding pendulum"   ,"dae2/dae2.dem.gateway.sce"   ];

subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;