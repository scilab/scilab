// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.


function subdemolist = wheel_demo_gateway()

    demopath = get_absolute_file_path("wheel.dem.gateway.sce");

    subdemolist = [_("Reading a precomputed trajectory")               ,"wheel1.dem.sce"  ; ..
    _("Simulation with ode (needs a Fortran compiler)") ,"wheel2.dem.sce"  ];

    subdemolist(:,2) = demopath + subdemolist(:,2);
    clear demopath;

endfunction

subdemolist = wheel_demo_gateway();
clear wheel_demo_gateway;