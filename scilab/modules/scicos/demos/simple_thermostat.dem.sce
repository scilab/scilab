//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

thispath = get_absolute_file_path("simple_thermostat.dem.sce");
// scicos_demostration(thispath+"/Simple_Thermostat.cosf");

// Open the cosf file with xcos
xcos(thispath+"/Simple_Thermostat.cosf");
clear thispath;
