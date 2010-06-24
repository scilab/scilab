// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released into the public domain

// Clear the functions defined in truck.sci to avoid warnings

clear truck_solve polyline truck_polynomial_curve cr2Tfjt cr2Tkf cr2Tfk display_truck_trajectory build_truck draw_truck polyline

exec(get_absolute_file_path("truck.dem.sce") + "/truck.sci",-1);

initial   = [-2;3;0.5235988;0;0;1];
final     = [0;0;0;0;0;0];
state     = truck_solve(initial,final);
my_handle = scf(100001);
clf(my_handle,"reset");
my_handle.pixmap = "off" ;
toolbar(my_handle.figure_id,"off") ;
display_truck_trajectory(state);
