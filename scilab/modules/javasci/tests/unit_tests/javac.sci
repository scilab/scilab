// ====================================================================

// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// ====================================================================
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
