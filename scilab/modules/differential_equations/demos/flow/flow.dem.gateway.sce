// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("flow.dem.gateway.sce");

subdemolist = ["Black hole"  ,"blackhole.dem.sce" ;
"Cylinder"    ,"cylinder.dem.sce"  ;
"Sphere"      ,"sphere.dem.sce"    ];

subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;