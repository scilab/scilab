//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//


demopath = get_absolute_file_path("cacsd.dem.gateway.sce");

subdemolist = ["LQG"                , "lqg/lqg.dem"
               "Mixed-sensitivity"  , "mixed.dem"
               "PID"                , "pid.dem"
               "Inverted pendulum"  , "pendulum/pendule.dem"
               "Flat systems"       , "flat/flat.dem.gateway.sce"
               "Tracking"           , "tracking/track.dem.sce"
               "Robust control"     , "robust/rob.dem"]

subdemolist(:,2) = demopath + subdemolist(:,2);
