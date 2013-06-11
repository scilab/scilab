// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function bike_demo1()
    thispath = get_absolute_file_path("bike1.dem.sce");

    exec(thispath + "macros/bike_show.sci");
    exec(thispath + "macros/velod.sci");
    exec(thispath + "macros/velodp.sci");
    exec(thispath + "macros/velo2.sci");

    x = read(thispath + "data/x1.dat",46,368,"(e10.4)");

    my_handle = scf(100001);
    clf(my_handle, "reset");

    bike_show(x(:,300:368), 1, 1, velo4p);
endfunction

bike_demo1();
clear bike_demo1;
