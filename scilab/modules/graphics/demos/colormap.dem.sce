// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Pierre MARECHAL
//
// This file is released into the public domain

my_path = SCI+"/modules/graphics/demos";
funcprot(0);
exec(my_path+"/colormap.dem.sci");
demo_colormap();
f=mopen(my_path+"/colormap.dem.sci");
demo_viewCode("colormap.dem.sci");
mclose(f);
