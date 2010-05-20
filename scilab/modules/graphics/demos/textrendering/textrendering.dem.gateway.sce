// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
//
// This file is released into the public domain

demopath = get_absolute_file_path("textrendering.dem.gateway.sce");

subdemolist = [                                   ..
	"Full LaTeX demo"         , "latex/fulltest.dem.sce"          ; ..
	"LaTeX demo with subplots"         , "latex/latexsubplot.dem.sce"          ; ..
	"LaTeX with graphics"    , "latex/simple.dem.sce"       ; ..
        "LaTeX with colored box" , "latex/coloredlabel.dem.sce" ; ..		    
	"MathML with graphics"          , "mathml/simple.dem.sce"           ];
//        "Full MathML demo"          , "mathml/fulltest.dem.sce"; ..
	
subdemolist(:,2) = demopath + subdemolist(:,2);
