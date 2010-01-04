//
// Scilab ( http://www.scilab.org/ ) 
// This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
// This file is distributed under the same license as the Scilab package.
//

if with_modelica_compiler() then

demopath = get_absolute_file_path("modelica_demos.dem.gateway.sce");

subdemolist = ["Ball on a Platform"                       , "ModelicaBlocks/ball_platform.dem.sce"           ; ..
               "Bouncing Ball     "                       , "ModelicaBlocks/modelica_bouncing_ball.dem.sce"  ; ..
               "Chaos Modelica"                           , "ModelicaBlocks/modelica_chaos.dem.sce"          ; ..
               "Hydraulics blocks"                        , "ModelicaBlocks/modelica_hydraulics.dem.sce"     ; ..
               "RLC circuit"                              , "ModelicaBlocks/modelica_rlc_circuit.dem.sce"  
              ];

subdemolist(:,2) = demopath + subdemolist(:,2);

end