//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ENPC
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

thispath = get_absolute_file_path("wheel1.dem.sce");

my_handle = scf(100001);
clf(my_handle,"reset");

// A precomputed value for
// x0=[0;%pi/2+0.1;0;5.0;0.0;4.0;0;0];

exec(thispath+"wheel_show.sci");
x=read(thispath+"x.wrt",8,301);

wheelg=wheelgs;
wheel_show(x);

// Clear variable and functions
clear wheelg wheelgs;
clear wheel_show wheeld test_wheel wheel_build_and_load get_wheel_rti;
