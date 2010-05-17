// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
// This file is released into the public domain

demopath = get_absolute_file_path("xcos.dem.gateway.sce");

subdemolist = ["Standard demos"                       , "standard.dem.gateway.sce"       ; ..
               "Control Systems"                      , "control_systems.dem.gateway.sce"    ; ..
               "Electrical Systems"                   , "electrical_systems.dem.gateway.sce" ; ..
               "Mechanical Systems"                   , "mechanical_systems.dem.gateway.sce" ;];
               
if with_modelica_compiler() then               
subdemolist = [subdemolist; ..
               "Modelica demos"                       , "modelica_demos.dem.gateway.sce"];
end

subdemolist = [subdemolist; ..
               "Old demos"                            , "old_demos.dem.gateway.sce"          ; ..
              ];

subdemolist(:,2) = demopath + subdemolist(:,2);
