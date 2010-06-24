// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file is released into the public domain

demopath = get_absolute_file_path("wheel.dem.gateway.sce");

subdemolist = ["Reading a precomputed trajectory"                                  ,"wheel1.dem.sce"  ; ..
               "Simulation with ode (needs f77 and link. may fail, see help link)" ,"wheel2.dem.sce"  ];

subdemolist(:,2) = demopath + subdemolist(:,2);
