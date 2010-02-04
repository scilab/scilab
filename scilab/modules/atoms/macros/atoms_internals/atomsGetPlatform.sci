// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function
// Operating system detection + archi

function [OSNAME,ARCH,LINUX,MACOSX,SOLARIS,BSD] = atomsGetPlatform()
	
	// Operating system detection
	
	if ~MSDOS then
		OSNAME   = unix_g("uname");
		MACOSX   = (strcmpi(OSNAME,"darwin") == 0);
		LINUX    = (strcmpi(OSNAME,"linux")  == 0);
		SOLARIS  = (strcmpi(OSNAME,"sunos")  == 0);
		BSD      = (regexp(OSNAME ,"/BSD$/") <> []);
	else
		MACOSX  = %F;
		LINUX   = %F;
		SOLARIS = %F;
		BSD     = %F;
	end
	
	if MSDOS then
		OSNAME = "windows";
	elseif LINUX then
		OSNAME = "linux";
	elseif MACOSX then
		OSNAME = "macosx";
	elseif SOLARIS then
		OSNAME = "solaris";
	elseif BSD then
		OSNAME = "bsd";
	end
	
	// Architecture
	
	[dynamic_info,static_info] = getdebuginfo();
	arch_info  = static_info(grep(static_info,"/^Compiler Architecture:/","r"))
	
	if ~isempty(arch_info) & (regexp(arch_info,"/\sX64$/","o") <> []) then
		ARCH = "64";
	else
		ARCH = "32";
	end
	
endfunction
