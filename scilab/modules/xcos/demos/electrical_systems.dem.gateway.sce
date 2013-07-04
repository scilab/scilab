// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2010 - DIGITEO
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [_("RLC Circuit")                      , "Electrical/rlc_circuit.dem.sce"           ; ..
_("Bridge Rectifer")                  , "Electrical/bridge_rectifier.dem.sce"      ; ..
_("Transformer")                      , "Electrical/transformer.dem.sce"           ; ..
_("Differential amplifier")           , "Electrical/differential_amp.dem.sce"  ; ..
_("OpAmp amplifier")                  , "Electrical/op_amplier.dem.sce"        ; ..
_("Switched capacitor integrator")    , "Electrical/switched_cap_filter.dem.sce"    ; ..
_("DC/DC Buck Converter")             , "Electrical/buck_conv.dem.sce"    ; ..
_("DC/DC Boost Converter")            , "Electrical/boost_conv.dem.sce"    ; ..
_("Colpitts oscillator")              , "Electrical/colpitts_osc.dem.sce"    ; ..
_("Logic AND gate")                   , "Electrical/logic_and.dem.sce"    ; ..
_("Logic NOR gate")                   , "Electrical/logic_nor.dem.sce"    ; ..
];

subdemolist(:,2) = SCI + "/modules/xcos/demos/" + subdemolist(:,2);
