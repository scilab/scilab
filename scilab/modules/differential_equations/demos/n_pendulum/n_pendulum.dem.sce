//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
// Copyright (C) ENPC
//
// This file is distributed under the same license as the Scilab package.
//

mode(-1);

path=pathconvert(get_absolute_file_path('n_pendulum.dem.sce'),%t,%t)	
exec(path+"n_pendulum.sci");
demo_pendulum()
