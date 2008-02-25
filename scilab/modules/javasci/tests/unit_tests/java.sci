// ====================================================================

// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// ====================================================================
function [rep,stat] = java(java_filename)

	if ~MSDOS then
		setenv('LD_LIBRARY_PATH','$LD_LIBRARY_PATH' + pathsep() + pwd());
		setenv('CLASSPATH','$CLASSPATH' + pathsep() + pwd() + pathsep() + '.');
		setenv('SHLIB_PATH','$SHLIB_PATH' + pathsep() + pwd() + pathsep() + '.');
	else 
		setenv('PATH',SCI+'\bin;' + getenv('PATH') + pathsep());
	end
	
	[path,fname,extension] = fileparts(java_filename);

	if MSDOS then
	  commandline = jre_path() + filesep() + 'bin\java -cp ' + ..
	                SCI + '\modules\javasci\jar\javasci.jar' + ..
	                pathsep() + '. ' + fname; 
	else
	  commandline = jre_path() + filesep() + 'bin/java -cp ' + ..
	                SCI + '/modules/javasci/jar/javasci.jar' + ..
	                pathsep() + '. -Djava.library.path=' + SCI + ..
	                '/modules/javasci/.libs/ ' + fname ;
	end
	
	[rep,stat] = unix_g(commandline);
  
endfunction
// ====================================================================
