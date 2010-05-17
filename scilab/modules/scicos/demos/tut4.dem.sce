//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

thispath = get_absolute_file_path("tut4.dem.sce");
exec(thispath+"/scicos_play.sci");
scicos_play(thispath+"/tut4");
clear thispath;
clear scicos_play;
