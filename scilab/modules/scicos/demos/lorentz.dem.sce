// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

thispath = get_absolute_file_path("lorentz.dem.sce");
// scicos_demostration(thispath+"/Lorentz.cosf");

//Open the cosf file with xcos
xcos(thispath+"/Lorentz.cosf");

clear thispath;
