// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA
// Copyright (C) 2009 - DIGITEO
//
// This file is released into the public domain

demopath = get_absolute_file_path("gui.dem.gateway.sce");

subdemolist = ["Dialogs"   ,"dialog.dem.sce" ; ..
	"Uicontrols 1" ,"uicontrol.dem.sce"      ];

if ~ usecanvas() then
	subdemolist = [ subdemolist ; "Uicontrols 2" ,"uicontrol_plot3d.dem.sce" ];
end

subdemolist(:,2) = demopath + subdemolist(:,2);
