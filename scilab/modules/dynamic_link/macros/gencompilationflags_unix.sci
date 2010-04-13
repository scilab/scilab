// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Sylvestre LEDRU
// Copyright (C) 2010 - DIGITEO - Sylvestre LEDRU
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt


function cmd = gencompilationflags_unix(ldflags, cflags, fflags, cc, flagsType)
	
	// This function is restricted to Linux/Unix user only
	
	if MSDOS then
	  error(msprintf(gettext("%s: Feature not available under Microsoft Windows.\n"),'gencompilationflags_unix'));
	  return;
	end
	
	[lhs,rhs] = argn(0);
	if rhs <> 5 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d expected.\n"),"gencompilationflags_unix",5));
		return
	end
	
	cmd=''
	
	// CFLAGS
	if cflags <> '' then
		cmd = cmd +" CFLAGS="""+cflags+""""
	end
	
	// CXXFLAGS ... use the same as C
	if cflags <> '' then
		cmd = cmd +" CXXFLAGS="""+cflags+""""
	end
	
	// LDFLAGS
	// Do not set the LDFLAGS See bug #4787
	if ldflags <> '' & flagsType <> "configure" then
		cmd = cmd +" LDFLAGS="""+ldflags+""""
	end
	
	// FFLAGS
	if fflags <> '' then
		cmd = cmd +" FFLAGS="""+fflags+""""
	end
	
	// CC
	if cc <> '' then
		cmd = cmd +" CC="""+cc+""""
	end
	
endfunction
