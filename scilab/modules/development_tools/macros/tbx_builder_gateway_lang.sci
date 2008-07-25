// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Run the gateway_path_path+"/sci_gateway/"+lang+"builder_gateway_"+lang+".sce" script if it exists
// See devtools_run_builder
function tbx_builder_gateway_lang(lang, gateway_path)
	oldpath = pwd();
	
	if(exists('gateway_path', 'local')) then
		chdir(gateway_path);
	end
	
	if(isdir(lang)) then
		chdir(lang);
		exec('builder_gateway_' + lang + '.sce', -1);
	end
	
	chdir(oldpath);
endfunction
