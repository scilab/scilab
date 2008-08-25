//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is distributed under the same license as the Scilab package.
//

funcprot(0);

my_demo_path = get_absolute_file_path("sphere.dem.sce");

exec(my_demo_path+"sphere.sci");
exec(my_demo_path+"sphere_interface.sci");
exesphere();

clear my_demo_path;

// clearglobal g_pente;
// clearglobal g_x g_y;
// clearglobal g_Vx g_Vy;
// clearglobal go_on eff;
// clearglobal g_alpha g_theta;
