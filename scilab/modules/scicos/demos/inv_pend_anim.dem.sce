//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

thispath = get_absolute_file_path("inv_pend_anim.dem.sce");
exec(thispath+"/PENDULUM_ANIM.sci");
exec(thispath+"/anim_pen.sci");

//scicos_demostration(thispath+"/pendulum_anim5.cosf");

// Open the cosf file with xcos
xcos(thispath+"/pendulum_anim5.cosf");
clear thispath;
