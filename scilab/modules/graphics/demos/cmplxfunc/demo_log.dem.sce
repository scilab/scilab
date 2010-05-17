//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2005-2008 - INRIA - Pierre MARECHAL <pierre.marechal@inria.fr>
//
// This file is distributed under the same license as the Scilab package.
//

// =============================================================================
// log
// =============================================================================

function demo_log()
	
	R     = 4;
	e     = 0.001;
	theta = 30;
	alpha = 60;
	
	my_handle = scf(100001);
	clf(my_handle,"reset");
	
	PlotCmplxFunc(R,e,"Disk","Ox",[40 20],"log",theta,alpha,[e,R]);
	
endfunction

demo_log()
demo_viewCode("demo_log.dem.sce");
clear demo_log;
