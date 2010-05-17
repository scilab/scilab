//
// Scilab ( http://www.scilab.org/ ) 
// This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("electrical_systems.dem.gateway.sce");

subdemolist = ["RLC Circuit"                      , "Electrical/rlc_circuit.dem.sce"           ; ..
               "Bridge Rectifer"                  , "Electrical/bridge_rectifier.dem.sce"      ; ..
               "Transformer"                      , "Electrical/transformer.dem.sce"           ; ..
               "Differential amplifier"           , "Electrical/differential_amp.dem.sce"  ; ..
               "OpAmp amplifier"                  , "Electrical/op_amplier.dem.sce"        ; ..
               "Switched capacitor integrator"    , "Electrical/switched_cap_filter.dem.sce"    ; ..
               "DC/DC Buck Converter"             , "Electrical/buck_conv.dem.sce"    ; ..
               "DC/DC Boost Converter"            , "Electrical/boost_conv.dem.sce"    ; ..
               "Colpitts oscillator"              , "Electrical/colpitts_osc.dem.sce"    ; ..
               "Logic AND gate"                   , "Electrical/logic_and.dem.sce"    ; ..
               "Logic NOR gate"                   , "Electrical/logic_nor.dem.sce"    ; ..
              ];

subdemolist(:,2) = demopath + subdemolist(:,2);
