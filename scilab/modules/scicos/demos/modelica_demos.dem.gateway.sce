//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("modelica_demos.gateway.sce");

subdemolist = ["Ball on a Platform"                       , "ball_platform.dem.sce"    ; ..
               "Bouncing Ball     "                       , "modelica_bouncing_ball.dem.sce"  ; ..
              ];

subdemolist(:,2) = demopath + subdemolist(:,2);
