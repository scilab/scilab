// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

thispath = get_absolute_file_path("discrete_kalman.dem.sce");
// scicos_demostration(thispath+"/Discrete-KalmanFilter.cosf");

//Open the cosf file with xcos
xcos(thispath+"/Discrete-KalmanFilter.cosf");

clear thispath;
