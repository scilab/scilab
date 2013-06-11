// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

//
// Sliding pendulum, curve #1
//

function demo_pendg1()

    exec(SCI+"/modules/differential_equations/demos/dae/dae2/pendg.sci");
    exec(SCI+"/modules/differential_equations/demos/dae/dae2/pendc1.sci");

    g    = 10;
    l    = 1;
    m    = 1;
    M    = 1;
    k    = 0.5;
    x0   = [1;1;0];
    u0   = [0;0;0];
    y0   = [x0;u0;0];
    ud0  = [0;-g;0];
    yd0  = [u0;ud0;0];

    t0   = 0;
    T    = t0:0.05:20;

    info = list([],0,[],[],[],0,0);
    atol = [0.0001;0.0001;0.0001;0.0001;0.0001;0.0001;0.001];
    rtol = atol;

    sol  = dassl([y0,yd0],t0,T,rtol,atol,pendg,info);

    H    = build_sliding_pendulum ()
    draw_sliding_pendulum(H,y0(1:3))

    //visualization of the result
    realtimeinit(0.05);realtime(0)
    for i=1:size(sol,2)
        realtime(i);
        if is_handle_valid(H) then
            draw_sliding_pendulum(H,sol(2:4,i))
        end
    end

endfunction

demo_pendg1();
clear demo_pendg1;