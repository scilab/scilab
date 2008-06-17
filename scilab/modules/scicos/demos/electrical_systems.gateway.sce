//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("electrical_systems.gateway.sce");

subdemolist = ["RLC Circuit"                              , "rlc.dem.sce"               ; ..
               "Bridge Rectifer"                          , "bridge_rectifier.dem.sce"  ; ..
              ];

subdemolist(:,2) = demopath + subdemolist(:,2);
