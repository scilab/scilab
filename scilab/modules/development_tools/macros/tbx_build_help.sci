// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Simple wrapper around xmltojar
function tbx_build_help(toolbox_name, help_lang_path)
	if(~exists(help_lang_path, 'local'))
		help_lang_path = pwd();
	end
	
	xmltojar(help_lang_path, toolbox_name);
endfunction
