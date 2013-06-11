// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("dynamic_link.dem.gateway.sce");
    add_demo(_("Dynamic link"), demopath+"dynamic_link.dem.gateway.sce");

    subdemolist = [_("Call a C function")       , "call_c.sce" ;
    _("Call a Fortran subroutine")      , "call_fortran.sce" ;];

    subdemolist(:,2) = demopath + subdemolist(:,2)
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
