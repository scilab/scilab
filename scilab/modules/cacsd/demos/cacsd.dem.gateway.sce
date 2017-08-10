// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
// Copyright (C) 2011 - DIGITEO - Allan CORNET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function subdemolist = demo_gateway()
    demopath = get_absolute_file_path("cacsd.dem.gateway.sce");
    add_demo(gettext("Control Systems - CACSD"), demopath + "cacsd.dem.gateway.sce");

    subdemolist = [_("LQG")                , "lqg/lqg.dem"
    _("Mixed-sensitivity")  , "mixed.dem"
    _("PID")                , "pid.dem"
    _("Inverted pendulum")  , "pendulum/pendule.dem"
    _("Flat systems")       , "flat/flat.dem.gateway.sce"
    _("Tracking")           , "tracking/track.dem.sce"
    _("Robust control")     , "robust/rob.dem"]

    subdemolist(:,2) = demopath + subdemolist(:,2);
endfunction

subdemolist = demo_gateway();
clear demo_gateway;
