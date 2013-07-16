// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) ENPC
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

mode(-1);
path = pathconvert(get_absolute_file_path("n_pendulum.dem.sce"),%t,%t)
exec(path + "n_pendulum.sci");

demo_pendulum()

clear path
clear demo_pendulum;
