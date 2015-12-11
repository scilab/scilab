// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2010 - DIGITEO
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [_("Basic controller")                     , "basic_controller.dem.sce"  ;..
_("Water tank")                           , "demo_watertank.dem.sce"      ; ..
_("Discrete Controller")                  , "discrete_controller.dem.sce" ;..
_("Kalman Filter")                        , "kalman.dem.sce"            ; ..
_("Discrete Kalman Filter")               , "discrete_kalman.dem.sce"   ; ..
_("Cont.Plant-Hybrid Observer")           , "cont_sys_disc_cont.dem.sce" ; ..
_("Temperature Controller")               , "temp_controller.dem.sce"    ; ..
_("Inverted pendulum")                    , "inverted_pendulum.dem.sce"  ; ..
_("Lorenz butterfly")                    , "lorenz.dem.sce"            ; ..
];

subdemolist(:,2) = SCI + "/modules/xcos/demos/" + subdemolist(:,2);
