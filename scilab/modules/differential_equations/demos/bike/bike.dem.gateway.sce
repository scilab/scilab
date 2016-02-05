// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
// Copyright (C) 2012 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = bike_demo_gateway()
    demopath = get_absolute_file_path("bike.dem.gateway.sce");

    subdemolist = [_("Unstable trajectory")  , "bike1.dem.sce" ; ..
    _("Stable trajectory")   , "bike2.dem.sce" ];

    subdemolist(:,2) = demopath + subdemolist(:,2);
endfunction

subdemolist = bike_demo_gateway();
clear bike_demo_gateway;