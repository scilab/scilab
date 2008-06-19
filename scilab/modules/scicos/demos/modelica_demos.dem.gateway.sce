//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("modelica_demos.dem.gateway.sce");

subdemolist = ["Ball on a Platform"                       , "ModelicaBlocks/ball_platform.dem.sce"           ; ..
               "Bouncing Ball     "                       , "ModelicaBlocks/modelica_bouncing_ball.dem.sce"  ; ..
               "Chaos Modelica"                           , "ModelicaBlocks/modelica_chaos.dem.sce"          ; ..
               "Hydraulics blocks"                        , "ModelicaBlocks/modelica_hydraulics.dem.sce"     ; ..
               "RLC circuit"                              , "ModelicaBlocks/modelica_rlc_circuit.dem.sce"    ; ..
               "FSM buck converter"                       , "ModelicaBlocks/modelica_fsm_buck.dem.sce"       ; ..
               "FSM boost converter"                      , "ModelicaBlocks/modelica_fsm_boost.dem.sce"      ; ..
              ];

subdemolist(:,2) = demopath + subdemolist(:,2);
