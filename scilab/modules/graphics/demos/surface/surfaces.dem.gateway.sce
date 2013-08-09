// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("surfaces.dem.gateway.sce");

subdemolist=["Surface with holes 1"    , "hole3d.sce"   ; ..
"Surface with holes 2"    , "hole3d1.sce"  ; ..
"Sphere"                  , "sphere.sce"   ; ..
"Shell"                   , "shell.sce"    ; ..
"Spiral"                  , "spiral.sce"   ; ..
"Rings"                   , "rings.sce"    ; ..
"Torus"                   , "torus.sce"    ; ..
"Torus 1"                 , "torus1.sce"   ; ..
"Moebius"                 , "moebius.sce"  ; ..
"Tube"                    , "tube.sce"     ; ..
"Black Hole"              , "bh.sce"       ; ..
"Riemann surface (n=2)"   , "cplxroot.sce" ];

subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;