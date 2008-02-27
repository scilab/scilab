// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

function [rep,stat] = java(java_filename)
	
	my_saved_path = pwd();
	
	if MSDOS then
		setenv('PATH',SCI+'\bin;' + getenv('PATH') + pathsep());
	end
	
	[path,fname,extension] = fileparts(java_filename);
	
	cd(path);
	
	if MSDOS then
		commandline = jre_path() + "\bin\java -cp " ..
			+ SCI + "\modules\javasci\jar\javasci.jar" ..
			+ pathsep() + ". " + fname;
	else
		if fileinfo(SCI+"/modules/javasci/.libs/libjavasci.so") <> [] then
			commandline = jre_path() +  "/bin/java -cp " ..
				+ SCI + "/modules/javasci/jar/javasci.jar" ..
				+ ":. -Djava.library.path=" ..
				+ SCI + "/modules/javasci/.libs/ " + fname ;
		
		elseif fileinfo(SCI+"/../../lib/scilab/libjavasci.so") <> [] then
			commandline = jre_path() +  "/bin/java -cp " ..
				+ SCI + "/modules/javasci/jar/javasci.jar" ..
				+ ":. -Djava.library.path=" ..
				+ SCI + "/../../lib/scilab/ " + fname ;
		
		else
			mprintf("libjavasci not found\n");
			return;
		end
		
	end
	
	[rep,stat] = unix_g(commandline);
	
	cd(my_saved_path);
	
endfunction
