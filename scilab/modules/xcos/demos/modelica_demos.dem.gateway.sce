// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2010 - DIGITEO
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

if with_modelica_compiler() then

    subdemolist = [_("Ball on a Platform")     , "ModelicaBlocks/ball_platform.dem.sce"           ; ..
    _("Bouncing Ball")          , "ModelicaBlocks/modelica_bouncing_ball.dem.sce"  ; ..
    _("Chaos Modelica")         , "ModelicaBlocks/modelica_chaos.dem.sce"          ; ..
    _("Hydraulics blocks")      , "ModelicaBlocks/modelica_hydraulics.dem.sce"     ; ..
    _("RLC circuit")            , "ModelicaBlocks/modelica_rlc_circuit.dem.sce"    ; ..
    _("Heat conduction in a rod")              , "ModelicaBlocks/modelica_heat_conduction.dem.sce"; ..
    _("Second order rotational system")        , "ModelicaBlocks/modelica_rotational_system.dem.sce"
    ];

    subdemolist(:,2) = SCI + "/modules/xcos/demos/" + subdemolist(:,2);

end

clear with_modelica_compiler;
