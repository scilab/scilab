// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released into the public domain

// Clear the functions defined in car.sci to avoid warnings
clear car_solve polyline car_polynomial_curve display_car_trajectory build_car draw_car polyline

exec(get_absolute_file_path("car.dem.sce") + "/car.sci",-1);

initial               =[3;3;0;0];
final                 =[0;0;0;0];
my_handle             = scf(100001);
clf(my_handle,"reset");
my_handle.pixmap      = "off" ;
toolbar(my_handle.figure_id,"off");
state=car_solve(initial,final);
display_car_trajectory(state);
