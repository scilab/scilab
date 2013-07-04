// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) 2008 - INRIA
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_wheel1()

    thispath = get_absolute_file_path("wheel1.dem.sce");

    my_handle = scf(100001);
    clf(my_handle,"reset");

    // A precomputed value for
    // x0=[0;%pi/2+0.1;0;5.0;0.0;4.0;0;0];

    exec(thispath+"wheel_show.sci");
    x = read(thispath+"x.wrt",8,301);

    wheelg = wheelgs;
    wheel_show(x);

endfunction

demo_wheel1();
clear demo_wheel1;
