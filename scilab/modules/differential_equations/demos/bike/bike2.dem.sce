// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function bike_demo2()

    thispath = get_absolute_file_path("bike2.dem.sce");

    exec(thispath+"macros/bike_show.sci");
    exec(thispath+"macros/velod.sci");
    exec(thispath+"macros/velo1.sci");

    x = read(thispath+"data/xdemo.dat",46,401,"(e10.4)");

    my_handle = scf(100001);
    clf(my_handle,"reset");

    bike_show(x(:,1:340),1.6,1.47,velo1);

endfunction

bike_demo2();
clear bike_demo2;