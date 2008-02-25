// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================
function [rep,stat] = javac(java_filename)

	Command='javac -deprecation -d '+ SCI + ..
	        '/modules/javasci/tests/unit_tests -cp ' + ..
	        SCI + '/modules/javasci/jar/javasci.jar' + ..
	        filesep() +'. ' + java_filename;
	        
	[rep,stat]=unix_g(Command);
	
	if (stat <> 0) then
	  warning('Error in the compilation of ' + filename);
    warning('Command was : ' + Command);
	end
	
endfunction
// ====================================================================
