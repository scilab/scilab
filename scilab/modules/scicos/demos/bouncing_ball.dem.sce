// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

thispath = get_absolute_file_path("bouncing_ball.dem.sce");
// scicos_demostration(thispath+"/Bouncing_ball.cosf");
// Open the cosf file with xcos
xcos(thispath+"/Bouncing_ball.cosf");
clear thispath;
