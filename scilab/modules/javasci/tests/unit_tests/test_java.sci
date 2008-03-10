// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function bOK = test_java(java_filename)
	
	bOK = %F;
	
	exec("SCI/modules/javasci/tests/unit_tests/javac.sci",-1);
	exec("SCI/modules/javasci/tests/unit_tests/java.sci" ,-1);
	
	[rep,stat] = javac(java_filename);
	
	if stat == 0 then
		[rep,stat] = java(java_filename);
		
		if stat == 0 then
			disp(rep);
			bOK = %T;
		end
		
		[path,fname,extension] = fileparts(java_filename);
		mdelete(fname+'.class');
	end
	
endfunction
