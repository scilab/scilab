//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("dae1.dem.gateway.sce");

subdemolist = ["Index 2"                    , "pend3d2.dem.sce"   ; ..
               "Index 1"                    , "pend3d1.dem.sce"   ; ..
               "Index 1 with stabilization" , "pend3d1s.dem.sce"  ]

subdemolist(:,2) = demopath + subdemolist(:,2);
