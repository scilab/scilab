// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

// Clear the functions defined in truck.sci to avoid warnings


function demo_truck()
    exec("SCI/modules/cacsd/demos/flat/truck.sci",-1);

    initial   = [-2;3;0.5235988;0;0;1];
    final     = [0;0;0;0;0;0];
    state     = truck_solve(initial,final);
    my_handle = scf(100001);
    clf(my_handle,"reset");
    toolbar(my_handle.figure_id,"off") ;
    display_truck_trajectory(state);

endfunction

demo_truck();
clear demo_truck;
