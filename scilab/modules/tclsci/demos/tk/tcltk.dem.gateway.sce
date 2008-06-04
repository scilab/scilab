//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("tcltk.dem.gateway.sce");

demolist = ["Color"  ,"color.dem.sce"      ; ..
			"Puzzle" ,"puzzle.dem.sce"     ; ..
			"Scroll" ,"scroll.dem.sce"     ; ..
			"Scale"  ,"scale.dem.sce"      ]
			
demolist(:,2) = demopath + demolist(:,2);
