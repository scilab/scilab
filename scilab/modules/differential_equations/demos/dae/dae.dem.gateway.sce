//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("dae.dem.gateway.sce");

subdemolist = ["Spherical pendulum" ,"dae1.dem.sce"   ; ..
               "Sliding pendulum"   ,"dae2.dem.sce"   ];

subdemolist(:,2) = demopath + subdemolist(:,2);
