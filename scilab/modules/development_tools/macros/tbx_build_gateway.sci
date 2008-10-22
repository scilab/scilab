// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Simon LIPP <simon.lipp@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Simple wrapper around ilib_build
function tbx_build_gateway(libname, names, files, gateway_path, libs, ldflags, ..
                       cflags, fflags, cc, makename, ismex)
	oldpath = pwd();
	
	if exists('gateway_path', 'local') then
		chdir(gateway_path);
	end
	
	if ~exists('libs', 'local') then libs = []; end
	if ~exists('ldflags', 'local') then ldflags = ""; end
	if ~exists('cflags', 'local') then cflags = ""; end
	if ~exists('fflags', 'local') then fflags = ""; end
	if ~exists('cc', 'local') then cc = ""; end
	if ~exists('makename', 'local') then makename = "Makelib"; end
	if ~exists('ismex', 'local') then ismex = %f; end
	
	ilib_build(libname, names, files, libs, makename, ldflags, cflags, ..
	           fflags, ismex, cc);
	
	chdir(oldpath);
endfunction