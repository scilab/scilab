// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("core.dem.gateway.sce");
    add_demo(_("Introduction: Getting started with Scilab"), demopath + "core.dem.gateway.sce");

    //subdemolist = [_("Getting started with Scilab")  ,"intro/intro.dem.sce"]
    subdemolist = [_("Scilab objects")                      ,"intro/scilab_objects.dem.sce"
    _("Operations and numerical primitives") ,"intro/operations.dem.sce"
    _("Online definition of function")       ,"intro/online_definition_fct.dem.sce"
    _("Optimization and simulation")         ,"intro/optimization_simulation.dem.sce"
    _("Systems: definition and analysis")    ,"intro/systems.dem.sce"
    _("Linear algebra")                      ,"intro/linear_algebra.dem.sce"
    _("Call an external routine")            ,"intro/external_routine.dem.sce"]

    subdemolist(:,2) = demopath + subdemolist(:,2);

endfunction

subdemolist = demo_gateway();
clear demo_gateway;
