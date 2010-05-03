// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

thispath = get_absolute_file_path("goto_demo.dem.sce");
// scicos_demostration(thispath+"/goto_demo.cos");

// Open the cosf file with xcos
xcos_demostration(thispath+"/goto_demo.cosf");

clear thispath;
