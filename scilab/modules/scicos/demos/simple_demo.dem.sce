// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

thispath = get_absolute_file_path("simple_demo.dem.sce");
// scicos_demostration(thispath+"/Simple_Demo.cosf");

// Open the cosf file with xcos
xcos(thispath+"/Simple_Demo.cosf");

clear thispath;
