// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2010 - DIGITEO
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [_("Standard demos")                       , "standard.dem.gateway.sce"       ; ..
_("Control Systems")                      , "control_systems.dem.gateway.sce"    ; ..
_("Electrical Systems")                   , "electrical_systems.dem.gateway.sce" ; ..
_("Mechanical Systems")                   , "mechanical_systems.dem.gateway.sce" ; ..
_("Modelica demos")                       , "modelica_demos.dem.gateway.sce"     ; ..
_("Old demos")                            , "old_demos.dem.gateway.sce"          ; ..
];

subdemolist(:,2) = SCI + "/modules/xcos/demos/" + subdemolist(:,2);
