// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

thispath = get_absolute_file_path("demo_watertank.dem.sce");
// scicos_demostration(thispath+"/demo_watertank.cosf");

//Open the cosf file with xcos
xcos(thispath+"/demo_watertank.cosf");
clear thispath;
