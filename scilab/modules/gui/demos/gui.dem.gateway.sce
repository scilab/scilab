//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) ????-2008 - INRIA
//
// This file is distributed under the same license as the Scilab package.
//

demopath = get_absolute_file_path("gui.dem.gateway.sce");

subdemolist = ["Dialogs"   ,"dialog.dem.sce"       ; ..
			"Uicontrols 1" ,"uicontrol.dem.sce"     ; ..
			"Uicontrols 2" ,"uicontrol_plot3d.dem.sce"     ]
			
subdemolist(:,2) = demopath + subdemolist(:,2);
