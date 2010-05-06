// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

thispath = get_absolute_file_path("tut1.dem.sce");
exec(thispath+"/scicos_play.sci");
scicos_play(thispath+"/tut1");
clear thispath;
clear scicos_play;
