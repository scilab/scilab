//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("flow.dem.gateway.sce");

subdemolist = ["Black hole"  ,"blackhole.dem.sce" ;
               "Cylinder"    ,"cylinder.dem.sce"  ;
               "Sphere"      ,"sphere.dem.sce"    ];

subdemolist(:,2) = demopath + subdemolist(:,2);
