// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

prot = funcprot();
funcprot(0);

my_demo_path = get_absolute_file_path("blackhole.dem.sce");
exec(my_demo_path+"create_interface.sci");
exec(my_demo_path+"blackhole.sci");
exec(my_demo_path+"blackhole_interface.sci");
exeblackhole();

funcprot(prot);
clear my_demo_path;
