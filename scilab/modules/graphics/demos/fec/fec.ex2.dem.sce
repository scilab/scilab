// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is released into the public domain

demopath = get_absolute_file_path("fec.ex2.dem.sce");
demo_run(demopath+"/fec.ex2");

f=mopen(demopath+"/fec.ex2");
demo_viewCode("fec.ex2");
mclose(f);
