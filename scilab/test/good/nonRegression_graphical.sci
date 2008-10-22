function nonRegression_graphical()

	// Scilab Demo Play GUI
	// This routine is part of sciGUI toolbox
	// Copyright (C) 2004 Jaime Urzua Grez
	// mailto:jaime_urzua@yahoo.com
	// rev. 0.2 2004/06/24
	
	// Modified by Pierre MARECHAL
	// Scilab team
	// Copyright INRIA
	// Date : 28 Dec 2005

	sciGUI_init()
	
	filename=SCI+'/tests/nonRegression_tests/nonRegression_graphical/nonRegression_graphical.dpf';
	
	ll=lines();lines(0);
	TCL_EvalStr('set editvartmp [sciGUIGenFileName ]');
	slideFile=strsubst(TMPDIR,'\','/')+'/'+TCL_GetVar('editvartmp');
	fileFile=strsubst(filename,'\','/');
	TCL_EvalStr('set demoplaytmp [sciGUIDemoPlay -1 ""'+fileFile+'"" ""'+slideFile+'"" ]');
	demoplaytmp=evstr(TCL_GetVar('demoplaytmp'));
	while %t
		realtimeinit(0.1);
		realtime(0);realtime(0.1);
		demoplayfind=find(winlist()==demoplaytmp);	
		if (demoplayfind==[]) then break; end
	end
	lines(ll(2),ll(1));
		
endfunction
