//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("control_systems.gateway.sce");

subdemolist = ["Kalman Filter"     , "kalman.dem.sce"    ; ..
               "Lorents Butterfly" , "lorents.dem.sce"  ];

subdemolist(:,2) = demopath + subdemolist(:,2);d
