//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("scicos.dem.gateway.sce");

subdemolist = ["Simple Thermostat"                    , "Simple_Thermostat.dem.sce"        ; ..
               "Continuous Plant discrete controller" , "Plant_DiscreteController.dem.sce" ; ..
               "Simple demo"                          , "Simple_Demo.dem.sce"              ; ..
               "Table lookup example"                 , "Table_Lookup.dem.sce"             ; ..
               "Threshold"                            , "Threshold_ZeroCrossing.dem.sce"   ; ..
               "Scilab Block"                         , "Scilab_Block.dem.sce"             ; ..
               "Thermostat"                           , "Temp.dem.sce"                     ; ..
               "Fibonacci"                            , "fibo.dem.sce"                     ; ..
               "Cont.Disc-Observer"                   , "Cont.Disc-Observer.dem.sce"       ; ..
               "Controller"                           , "Controller.dem.sce"               ; ..
               "System-Observer"	                  , "System-Observer.dem.sce"          ; ..
               "Synchro Block demo"                   , "Ifsub.dem.sce"                    ; ..
               "Kalman filter"                        , "Kalman_1.dem.sce"                 ; ..
               "Inverted Pendulum"                    , "Inverted_pendulum.dem.sce"        ; ..
               "Lorentz attractor"                    , "Lorentz.dem.sce"                  ; ..
               "Datatype"                             , "demo_Datatype.dem.sce"            ; ..
               "Digital Pulse Generator"              , "demo_flipflop.dem.sce"            ; ..
               "WaterTank"                            , "demo_watertank.dem.sce"           ; ..
               "Goto/From Demonstration"              , "goto_demo.dem.sce"                ; ..
               "Live tutorials"                       , "live.dem.gateway.sce"             ];

subdemolist(:,2) = demopath + subdemolist(:,2);
