//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("dae.dem.gateway.sce");

subdemolist = ["Spherical pendulum" ,"dae1/pend3d1.dem.sce" ; 
               "Sliding pendulum"   ,"dae2/dae2.dem.gateway.sce"   ];

subdemolist(:,2) = demopath + subdemolist(:,2);
