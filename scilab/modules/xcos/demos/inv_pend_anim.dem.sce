// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009-2010 - DIGITEO
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

function demo_xcospendul()
    exec("SCI/modules/xcos/demos/PENDULUM_ANIM.sci",-1);
    exec("SCI/modules/xcos/demos/anim_pen.sci",-1);
    xcos("SCI/modules/xcos/demos/pendulum_anim5.zcos");
endfunction

demo_xcospendul();
clear demo_xcospendul;


