// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2010 - DIGITEO
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

subdemolist = [_("Simple Demo")                          , "simple_demo.dem.sce"    ; ..
_("Bouncing Balls")                       , "bounce.dem.sce"            ; ..
_("Simple Thermostat")                    , "simple_thermostat.dem.sce" ; ..
_("Table Lookup")                         , "table_lookup.dem.sce"      ; ..
_("Signal Builder")                       , "signal_builder.dem.sce"    ; ..
_("Fibonacci Numbers")                    , "fibonacci.dem.sce"         ; ..
_("Scilab block")                         , "scilab_block.dem.sce"      ; ..
_("Xcos data types")                    , "data_type.dem.sce"         ; ..
_("Zero Crossing")                        , "zero_crossing.dem.sce"     ; ..
_("_IF_ Xcos block")                    , "if_xcos_block.dem.sce"   ; ..
_("Goto/From blocks")                     , "goto_from.dem.sce"         ; ..
];

subdemolist(:,2) = SCI + "/modules/xcos/demos/" + subdemolist(:,2);
