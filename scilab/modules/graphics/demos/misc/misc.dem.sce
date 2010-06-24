// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file is released into the public domain

my_path = SCI+"/modules/graphics/demos/misc";
funcprot(0);
exec(my_path+"/misc.dem.sci");funcprot(1);
demo_misc();
f=mopen(my_path+"/misc.dem.sci");
demo_viewCode("misc.dem.sci");
mclose(f);
