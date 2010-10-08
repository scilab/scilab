// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
//
// This file is released under the 3-clause BSD license. See COPYING-BSD.

thispath = get_absolute_file_path("scilab_block.dem.sce");
// scicos_demostration(thispath+"/Scilab_Block.cosf");

//Open the cosf file with xcos
xcos(thispath+"/Scilab_Block.cosf");

clear thispath;
