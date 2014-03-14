// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("matplot.dem.gateway.sce");

subdemolist = [                                   ..
_("Classical Matplot")      , "classical.dem.sce"    ; ..
_("Classical Matplot1")    , "classical_1.dem.sce"       ; ..
_("Mandelbrot")    , "mandelbrot.dem.sce"       ; ..
_("Java Image")    , "java_image.dem.sce"       ];

subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;