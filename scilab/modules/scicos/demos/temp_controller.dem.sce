// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

thispath = get_absolute_file_path("temp_controller.dem.sce");
// scicos_demostration(thispath+"/Temperature_Controller.cosf");

// Open the cosf file with xcos
xcos(thispath+"/Temperature_Controller.cosf");

clear thispath;
