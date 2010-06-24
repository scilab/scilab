// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

demopath = get_absolute_file_path("live.dem.gateway.sce");

subdemolist = ["Simple block construction"                , "tut1.dem.sce"   ; ..
               "Use of Super block"                       , "tut2.dem.sce"   ; ..
               "Use of symbolic parameters"               , "tut3.dem.sce"   ; ..
               "Constructing a new block"                 , "tut4.dem.sce"   ; ..
               "the Scifunc block"                        , "tut5.dem.sce"   ; ..
               "The Cblock2 block"                        , "tut6.dem.sce"   ];

subdemolist(:,2) = demopath + subdemolist(:,2);
