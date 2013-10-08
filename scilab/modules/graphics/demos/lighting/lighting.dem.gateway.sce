// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2013 - Pedro SOUZA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("lighting.dem.gateway.sce");

subdemolist = [                                   ..
"Simple light"      , "simple_light.dem.sce"    ; ..
"Rotating light"    , "rot_light.dem.sce"       ; ..
"Rotating plot3d"    , "rot_plot3d.dem.sce"       ; ..
"Colored light"     , "color_light.dem.sce"     ; ..
"Directional light" , "dir_light.dem.sce"       ];

subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;