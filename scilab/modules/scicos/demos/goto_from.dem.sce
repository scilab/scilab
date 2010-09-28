// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

thispath = get_absolute_file_path("goto_from.dem.sce");
//scicos_demostration(thispath+"/goto_demo.cosf");

// Open the cosf file with xcos
xcos(thispath+"/goto_demo.cosf");

clear thispath;
