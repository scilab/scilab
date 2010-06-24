// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

demopath = get_absolute_file_path("mechanical_systems.dem.gateway.sce");

subdemolist = ["Automotive Suspension"                    , "susp.dem.sce"              ; ..
              ];

subdemolist(:,2) = demopath + subdemolist(:,2);
