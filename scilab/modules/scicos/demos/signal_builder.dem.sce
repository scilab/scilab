//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

thispath = get_absolute_file_path("signal_builder.dem.sce");
// scicos_demostration(thispath+"/Signal_Builder.cosf");

// Open the cosf file with xcos
xcos(thispath+"/Signal_Builder.cosf");
clear thispath;
