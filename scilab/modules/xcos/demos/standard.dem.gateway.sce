//
// Scilab ( http://www.scilab.org/ )
// This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("standard.dem.gateway.sce");

subdemolist = ["Simple Demo"                          , "simple_demo.dem.sce"    ; ..
               "Bouncing Balls"                       , "bounce.dem.sce"            ; ..
               "Simple Thermostat"                    , "simple_thermostat.dem.sce" ; ..
               "Table Lookup"                         , "table_lookup.dem.sce"      ; ..
               "Signal Builder"                       , "signal_builder.dem.sce"    ; ..
               "Fibonacci Numbers"                    , "fibonacci.dem.sce"         ; ..
               "Scilab block"                         , "scilab_block.dem.sce"      ; ..
               "Scicos data types"                    , "data_type.dem.sce"         ; ..
               "Zero Crossing"                        , "zero_crossing.dem.sce"     ; .. 
               "_IF_ Scicos block"                    , "if_scicos_block.dem.sce"   ; ..
               "Goto/From blocks"                     , "goto_from.dem.sce"         ; ..
              ];

subdemolist(:,2) = demopath + subdemolist(:,2);

//**               "Bouncing Ball"                        , "bouncing_ball.dem.sce"  ; ..
//**               "Space Balls"                          , "bounce.dem.sce"            ; ..
