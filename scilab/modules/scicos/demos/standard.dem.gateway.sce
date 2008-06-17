//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("standard.dem.gateway.sce");

subdemolist = ["Simple Demo"                              , "simple_demo.dem.sce"    ; ..
               "Bouncing Ball"                            , "bouncing_ball.dem.sce"  ; ..
               "Space Balls"                              , "bounce.dem.sce"     ; ..
               "Flip-Flop Logic"                          , "demo_flipflop.dem.sce " ; ..
               "Signal Builder"                           , "bignal_builder.dem.sce" ; ..
               "Table Lookup"                             , "table_lookup.dem.sce"   ; ..
               "The Cblock2 block"                        , "goto_demo.dem.sce"   ];

subdemolist(:,2) = demopath + subdemolist(:,2);
