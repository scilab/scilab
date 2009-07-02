// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

function dir_created = atomsExtract(archive_in,dir_out)
	
	// Check input parameters number
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsExtract",2));
	end
	
	// Check input parameters type
	// =========================================================================
	
	if type(archive_in) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsExtract",1));
	end
	
	if type(dir_out) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: Single string expected.\n"),"atomsExtract",2));
	end
	
	// Check input parameters size
	// =========================================================================
	
	if size(archive_in,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsExtract",1));
	end
	
	if size(dir_out,"*") <> 1 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: Single string expected.\n"),"atomsExtract",2));
	end
	
	// Check input parameters value
	// =========================================================================
	
	if regexp(archive_in,"/(\.tar\.gz|\.tgz|\.zip)$/","o") == [] then
		error(msprintf(gettext("%s: Wrong value for input argument #%d: Single string that end with .tar.gz, .tgz or .zip expected.\n"),"atomsExtract",1));
	end
	
	if fileinfo(archive_in) == [] then
		error(msprintf(gettext("%s: The file ""%s"" doesn''t exist or is not read accessible.\n"),"atomsExtract",archive_in));
	end
	
	if ~ isdir(dir_out) then
		error(msprintf(gettext("%s: The directory ""%s"" doesn''t exist.\n"),"atomsExtract",dir_out));
	end
	
	// Operating system detection
	// =========================================================================
	
	if ~MSDOS then
		OSNAME = unix_g('uname');
		MACOSX = (strcmpi(OSNAME,"darwin") == 0);
		LINUX  = (strcmpi(OSNAME,"linux") == 0);
	else
		MACOSX = %F;
		LINUX  = %F;
	end
	
	if MSDOS then
		OSNAME = "windows";
	elseif LINUX then
		OSNAME = "linux";
	elseif MACOSX then
		OSNAME = "macosx";
	end
	
	// Get the list of directories before the extraction
	// =========================================================================
	dirs_before = atomsListDir(dir_out);
	
	// Build the extract command
	// =========================================================================
	
	if ( LINUX | MACOSX ) & regexp(archive_in,"/(\.tar\.gz|\.tgz)$/","o") <> [] then
		
		extract_cmd = "tar xzf "+ archive_in + " -C """+ dir_out + """";
		
	elseif regexp(archive_in,"/\.zip$/","o") <> [] then
		
		if MSDOS then
			extract_cmd = getshortpathname(pathconvert(SCI+"/tools/zip/unzip.exe",%F));
		else
			extract_cmd = "unzip";
		end
		
		extract_cmd = extract_cmd + " -q """ + archive_in + """ -d """ + pathconvert(dir_out,%F) +"""";
		
	end
	
	[rep,stat,err] = unix_g(extract_cmd);
	
	if stat ~= 0 then
		error(msprintf(gettext("%s: The extraction of the archive ''%s'' has failed.\n"),"atomsExtract",archive_in));
	end
	
	// Get the list of directories after the extraction
	// =========================================================================
	
	dirs_after = atomsListDir(dir_out);
	
	
	// Get the name of the created directory
	// =========================================================================
	
	dir_created = [];
	
	for j=1:size(dirs_after,"*")
		if find(dirs_after(j) == dirs_before) == [] then
			dir_created = dirs_after(j);
			break;
		end
	end
	
endfunction


// =============================================================================
// Just get the list of the directories present in the current directory
// =============================================================================

function result = atomsListDir(path)
	
	// Init the output argument
	// =========================================================================
	result = [];
	
	// Save the initial path
	// =========================================================================
	initialpath = pwd();
	
	chdir(path);
	items  = listfiles();
	
	// Loop on items
	// =========================================================================
	for i=1:size(items,"*")
		if isdir(items(i)) then
			result = [ result ; items(i) ];
		end
	end
	
	// Go to the initial location
	// =========================================================================
	chdir(initialpath);
	
endfunction
