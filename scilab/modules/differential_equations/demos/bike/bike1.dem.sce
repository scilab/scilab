//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file is distributed under the same license as the Scilab package.
//

thispath = get_absolute_file_path("bike1.dem.sce");

exec(thispath+"macros/bike_show.sci");
exec(thispath+"macros/velod.sci");
exec(thispath+"macros/velodp.sci");
exec(thispath+"macros/velo2.sci");

x = read(thispath+"data/x1.dat",46,368,"(e10.4)");

my_handle = scf(100001);
clf(my_handle,"reset");

bike_show(x(:,300:368),1,1,velo4p);

// Clear variable and functions
clear x bike_show get_velo_rti velo1 velo4p poteau arbre velodp velod;
