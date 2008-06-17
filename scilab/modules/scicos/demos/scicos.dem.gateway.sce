//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("scicos.dem.gateway.sce");

subdemolist = ["Standard demos"                       , "standard.dem.gateway.sce"       ; ..
               "Control Systems"                      , "control_systems.gateway.sce"    ; ..
               "Electrical Systems"                   , "electrical_systems.gateway.sce" ; ..
               "Modelica demos"                       , "modelica_demos.gateway.sce"     ; ..
               "Old demos"                            , "old_demos.gateway.sce"          ; ..
              ];

subdemolist(:,2) = demopath + subdemolist(:,2);
