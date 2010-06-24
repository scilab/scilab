// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

thispath = get_absolute_file_path("table_lookup.dem.sce");
//scicos_demostration(thispath+"/Table_Lookup.cosf");

// Open the cosf file with xcos
xcos(thispath+"/Table_Lookup.cosf");

clear thispath;
