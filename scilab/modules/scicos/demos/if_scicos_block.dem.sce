// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released into the public domain

thispath = get_absolute_file_path("if_scicos_block.dem.sce");
//scicos_demostration(thispath+"/IF_block.cosf");

// Open the cosf file with xcos
xcos(thispath+"/IF_block.cosf");

clear thispath;
