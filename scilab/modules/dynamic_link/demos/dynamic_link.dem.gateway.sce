// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2011 - DIGITEO - Allan CORNET
// Copyright (C) 2018 - ESI Group - Clement DAVID
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("dynamic_link.dem.gateway.sce");
    _("Dynamic link");  // lets gettext() harvesting it
    add_demo("Dynamic link", demopath+"dynamic_link.dem.gateway.sce");

    subdemolist = [_("Call a C function")                   , "call_c.sce" ;
    _("Call a C function (using C standard library)")       , "call_c_stdlib.sce" ;
    _("Call a Fortran subroutine")                          , "call_fortran.sce" ;
    _("Call a Fortran subroutine (with Fortran intrinsics)"), "call_fortran_stdlib.sce" ;
    _("Call a C++ function")                                , "call_cxx.sce" ;
    _("Call a C++ function (using C++ Standard Library)")   , "call_cxx_stdlib.sce" ;];

    subdemolist(:,2) = demopath + subdemolist(:,2)
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
