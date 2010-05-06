// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

thispath = get_absolute_file_path("flip_flop_logic.dem.sce");
//scicos_demostration(thispath+"/demo_flipflop.cosf");

// Open the cosf file with xcos
xcos(thispath+"/demo_flipflop.cosf");

clear thispath;
