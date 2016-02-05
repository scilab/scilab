// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_car()

    exec("SCI/modules/cacsd/demos/flat/car.sci",-1);

    initial               =[3;3;0;0];
    final                 =[0;0;0;0];
    my_handle             = scf(100001);
    clf(my_handle,"reset");
    toolbar(my_handle.figure_id,"off");
    state=car_solve(initial,final);
    display_car_trajectory(state);
endfunction

demo_car();
clear demo_car;
