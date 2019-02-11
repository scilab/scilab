// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2019 - Samuel GOUGEON
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

demopath = get_absolute_file_path("technical.dem.gateway.sce");

subdemolist = [
"Bode"               , "bode.dem.sce"
"Phase plot"         , "phaseplot.dem.sce"
"Gain plot"          , "gainplot.dem.sce"
"Nyquist"            , "nyquist.dem.sce"
"SV plot"            , "svplot.dem.sce"
"Hall chart"         , "hallchart.dem.sce"
"Nichols chart"      , "nicholschart.dem.sce"
"Black-Nichols"      , "black.dem.sce"
"s-grid"             , "sgrid.dem.sce"
"z-grid"             , "zgrid.dem.sce"
];

subdemolist(:,2) = demopath + subdemolist(:,2);
clear demopath;
