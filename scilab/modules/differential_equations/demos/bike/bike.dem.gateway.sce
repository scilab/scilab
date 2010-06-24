// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file is released into the public domain

demopath = get_absolute_file_path("bike.dem.gateway.sce");

subdemolist = ["unstable trajectory"  , "bike1.dem.sce" ; ..
               "stable trajectory!"   , "bike2.dem.sce" ];

subdemolist(:,2) = demopath + subdemolist(:,2);
