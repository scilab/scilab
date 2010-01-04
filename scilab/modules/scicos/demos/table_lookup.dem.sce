//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

thispath = get_absolute_file_path("table_lookup.dem.sce");
//scicos_demostration(thispath+"/Table_Lookup.cosf");

// Open the cosf file with xcos
xcos(thispath+"/Table_Lookup.cosf");
clear thispath;
