// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

demopath = get_absolute_file_path("dae2.dem.gateway.sce");

subdemolist = ["parabola"          , "pendg1.dem.sce"   ; ..
               "Upside down camel" , "pendg2.dem.sce"  ]

subdemolist(:,2) = demopath + subdemolist(:,2);
