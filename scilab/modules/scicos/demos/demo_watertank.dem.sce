//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

thispath = get_absolute_file_path("demo_watertank.dem.sce");
// scicos_demostration(thispath+"/demo_watertank.cosf");

//Open the cosf file with xcos
xcos(thispath+"/demo_watertank.cosf");

clear thispath;
