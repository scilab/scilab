// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Run the help_path+"/"+lang+"/build_help.sce" script if it exists
// See devtools_run_builder
function tbx_builder_help_lang(lang, help_path)
	oldpath = pwd();
	
	if(exists('help_path', 'local')) then
		chdir(help_path);
	end
	
	if(isdir(lang)) then
		chdir(lang);
		exec('build_help.sce', -1);
	end
	
	chdir(oldpath);
endfunction	
