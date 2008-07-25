// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Run the toolbox_path+"/sci_gateway/builder_gateway.sce" script if it exists
// See devtools_run_builder
function tbx_builder_gateway(toolbox_path)
	if ~haveacompiler() then
	  error(gettext('This toolbox requires a compiler to build.'));
	end
	mprintf(gettext('Building gateway...\n'));
	devtools_run_builder(toolbox_path, 'sci_gateway', 'builder_gateway.sce');
endfunction
