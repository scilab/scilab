// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Run the src_path+"/src/"+lang+"builder_"+lang+".sce" script if it exists
// See devtools_run_builder
function tbx_builder_src_lang(lang, src_path)
	oldpath = pwd();
	
	if(exists('src_path', 'local')) then
		chdir(src_path);
	end
	
	if(isdir(lang)) then
		chdir(lang);
		exec('builder_' + lang + '.sce');
	end
	
	chdir(oldpath);
endfunction
