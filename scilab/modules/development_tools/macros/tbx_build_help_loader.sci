// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Generate a loader file for the help
function tbx_build_help_loader(toolbox_title, help_lang_path)
	oldpath = pwd();
	
	if(exists('help_lang_path', 'local')) then
		chdir(help_lang_path);
	end
	
	loader = [
	  "// ====================================================================",
	  "// Copyright INRIA 2008",
	  "// Allan CORNET",
	  "// ====================================================================",
	  "path = get_absolute_file_path(''addchapter.sce'');",
	  "add_help_chapter(''"+toolbox_title+"'',path,%F);",
	  "clear path add_help_chapter;",
	  "// ===================================================================="
	];

	mputl(loader, 'addchapter.sce');
	chdir(oldpath);
endfunction
