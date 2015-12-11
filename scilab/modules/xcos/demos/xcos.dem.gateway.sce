// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2010 - DIGITEO
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()

    demopath = get_absolute_file_path("xcos.dem.gateway.sce");
    add_demo(gettext("Xcos"), demopath + "xcos.dem.gateway.sce");

    subdemolist = [_("Standard demos")      , "standard.dem.gateway.sce"       ; ..
    _("Control Systems")     , "control_systems.dem.gateway.sce"    ; ..
    _("Electrical Systems")  , "electrical_systems.dem.gateway.sce" ; ..
    _("Mechanical Systems")  , "mechanical_systems.dem.gateway.sce" ;];

    if with_modelica_compiler() then
        subdemolist = [subdemolist; ..
        _("Modelica demos")      , "modelica_demos.dem.gateway.sce"];
    end

    subdemolist = [subdemolist; ..
    _("Event demos")         , "event.dem.gateway.sce" ; ..
    _("Old demos")           , "old_demos.dem.gateway.sce"];

    subdemolist(:,2) = demopath + subdemolist(:,2);
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
