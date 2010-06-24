// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

demopath = get_absolute_file_path("control_systems.dem.gateway.sce");

subdemolist = ["Basic controller"                     , "basic_controller.dem.sce"  ;..
               "Water tank"                           , "demo_watertank.dem.sce"      ; ..
               "Discrete Controller"                  , "discrete_controller.dem.sce" ;..
               "Kalman Filter"                        , "kalman.dem.sce"            ; ..
               "Discrete Kalman Filter"               , "discrete_kalman.dem.sce"   ; ..
               "Cont.Plant-Hybrid Observer"           , "cont_sys_disc_cont.dem.sce" ; ..
               "Temperature Controller"               , "temp_controller.dem.sce"    ; ..
               "Inverted Pendulum"                    , "inverted_pendulum.dem.sce"  ; ..
               "Animated Inv.Pendulum"                , "inv_pend_anim.dem.sce"  ; ..
               "Lorentz butterfly"                    , "lorentz.dem.sce"            ; .. 
              ];

subdemolist(:,2) = demopath + subdemolist(:,2);
