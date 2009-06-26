// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Installation of a toolbox

function result = atomsInstall(packages,allusers)
	
	result = [];
	
	// Save the initial path
	// =========================================================================
	initialpath = pwd();
	
	// Get scilab version (needed for later)
	// =========================================================================
	sciversion = strcat(string(getversion("scilab")) + ".");
	
	// Check input parameters
	// =========================================================================
	
	rhs = argn(2);
	
	if rhs < 1 | rhs > 2 then
		error(msprintf(gettext("%s: Wrong number of input arguments: %d to %d expected.\n"),"atomsInstall",1,2))
	end
	
	if type(packages) <> 10 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: String array expected.\n"),"atomsInstall",1));
	end
	
	packages = stripblanks(packages);
	
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
	
	// Verbose Mode ?
	// =========================================================================
	if strcmpi(atomsGetConfig("Verbose"),"True") == 0 then
		VERBOSE = %T;
	else
		VERBOSE = %F;
	end
	
	// Install for all users or just for me ?
	// =========================================================================
	
	if rhs == 1 then
		// By default, install for all users (if we have write access of course !)
		if atomsAUWriteAccess() then
			allusers = %T; 
		else
			allusers = %F;
		end
	
	else
		// Just check if it's a boolean
		if type(allusers) <> 4 then
			chdir(initialpath);
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsInstall",2));
		end
		
		// Check if we have the write access
		if allusers & ~ atomsAUWriteAccess() then
			chdir(initialpath);
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstall",2,pathconvert(SCI+"/.atoms")));
		end
	end
	
	// Define the "archives" directory path
	// Create it if it's not exist
	// =========================================================================
	
	if allusers then
		archives_directory = pathconvert(SCI+"/contrib/archives");
	else
		archives_directory = pathconvert(SCIHOME+"/atoms/archives");
	end
	
	if ~ isdir( archives_directory ) then
		status = mkdir( archives_directory );
		if status <> 1 then
			chdir(initialpath);
			error(msprintf( ..
				gettext("%s: The directory ""%s"" cannot been created, please check if you have write access on this directory.\n"),..
				"atomsInstall", ..
				archives_directory));
		end
	end
	
	// Get the install list
	// =========================================================================
	[install_package_list,dependency_tree] = atomsInstallList(packages);
	
	// Loop on install_package_list to print if a package has to be installed
	// or not
	// =========================================================================
	if VERBOSE 
		for i=1:size(install_package_list(:,1),"*")
			if install_package_list(i,1) == "+" then
				mprintf("\t%s (%s) will be installed\n",install_package_list(i,3),install_package_list(i,4));
			elseif install_package_list(i,1) == "~" then
				mprintf("\t%s (%s) is already installed and up-to-date\n",install_package_list(i,3),install_package_list(i,4));
			end
		end
	end
	
	// Now really install the packages
	// =========================================================================
	
	for i=1:size(install_package_list(:,1),"*")
		
		this_package_name    = install_package_list(i,3);
		this_package_version = install_package_list(i,4);
		
		this_package_details = dependency_tree(this_package_name+" - "+this_package_version);
		
		if install_package_list(i,1) <> "+" then
			continue;
		end
		
		if VERBOSE then
			mprintf("\tInstalling %s (%s) ...",this_package_name,this_package_version);
		end
		
		// Define the path of the directory where will be installed this toolbox
		// =====================================================================
		
		if allusers then
			atoms_directory = pathconvert(SCI+"/contrib/"+this_package_name);
		else
			atoms_directory = pathconvert(SCIHOME+"/atoms/"+this_package_name);
		end
		
		// Create the parent directory of this toolbox if it's not already exist
		// =====================================================================
		
		if ~ isdir( atoms_directory ) then
			status = mkdir( atoms_directory );
			if status <> 1 then
				chdir(initialpath);
				error(msprintf( ..
					gettext("%s: The directory ""%s"" cannot been created, please check if you have write access on this directory.\n"),..
					atoms_directory));
			end
		end
		
		// Define the path of the downloaded file
		// =====================================================================
		
		fileout = pathconvert(atoms_directory+this_package_details(OSNAME+"Name"),%F);
		filein  = this_package_details(OSNAME+"Url");
		
		// Get the package
		// =====================================================================
		
		if MSDOS then
			download_cmd =  getshortpathname(pathconvert(SCI+"/tools/curl/curl.exe",%F)) +" -s """+ filein + """ -o """ + fileout + """";
		else
			download_cmd = "wget "+ filein + " -O " + fileout;
		end
		
		[rep,stat,err] = unix_g(download_cmd)
		
		if stat ~= 0 then
			disp(download_cmd);
			disp(err);
		end
		
		// Check the md5sum
		// =====================================================================
		
		filemd5 = getmd5(fileout);
		
		if filemd5 <> this_package_details(OSNAME+"Md5") then
			chdir(initialpath);
			error(msprintf(gettext("%s: The downloaded binary file (%s) doesn''t check the MD5SUM.\n"),"atomsInstall",fileout));
		end
		
		// unarchive it
		// =====================================================================
		
		chdir( atoms_directory );
		
		// get the list of directories before unarchive
		dir_list_before = atomsListDir();
		
		if ( LINUX | MACOSX ) & regexp(fileout,"/\.tar\.gz$/","o") <> [] then
			
			extract_cmd = "tar xzf "+ fileout + " -C """+ atoms_directory + """";
			
		elseif regexp(fileout,"/\.zip$/","o") <> [] then
			
			if MSDOS then
				extract_cmd = getshortpathname(pathconvert(SCI+"/tools/zip/unzip.exe",%F));
			else
				extract_cmd = "unzip";
			end
			
			extract_cmd = extract_cmd + " -q """ + fileout + """ -d """ + pathconvert(atoms_directory,%F) +"""";
			
		else
			chdir(initialpath);
			error(msprintf(gettext("%s: internal error, the archive ""%s"" cannot be extracted on this operating system.\n"),"atomsInstall",fileout));
		
		end
		
		[rep,stat,err] = unix_g(extract_cmd);
		
		if stat ~= 0 then
			disp(extract_cmd);
			disp(err);
			chdir(initialpath);
			error(msprintf(gettext("%s: internal error, the extraction of the archive ""%s"" has failed.\n"),"atomsInstall",fileout));
		end
		
		// get the list of directories after unarchive
		dir_list_after = atomsListDir();
		
		// Get the name of the created directory
		// =====================================================================
		
		unarchive_dir = "";
		
		for j=1:size(dir_list_after,"*")
			if find(dir_list_after(j) == dir_list_before) == [] then
				unarchive_dir = dir_list_after(j);
				break;
			end
		end
		
		// Rename the created directory
		// =====================================================================
		
		if MSDOS then
			rename_cmd = "rename";
		else
			rename_cmd = "mv";
		end
		
		rename_cmd = rename_cmd+" """+unarchive_dir+""" """+this_package_version+"""";
		
		[rep,stat]=unix_g(rename_cmd)
		
		if stat <> 0 then
			disp(rename_cmd);
			chdir(initialpath);
			error(msprintf(gettext("%s: Error while creating the directory ''%s''.\n"),"atomsInstall",pathconvert(atoms_directory+this_package_version)));
		end
		
		// Register the successfully installed package
		// =====================================================================
		
		if install_package_list(i,2) == "U" then
			// Intentionnaly Installed
			this_package_status = "I";
		else
			// Automaticaly installed
			this_package_status = "A";
		end
		
		atomsInstallRegister(this_package_name,this_package_version,this_package_status,allusers);
		
		// Autoload the toolbox unless precised
		// =====================================================================
		
		if ~ (atomsGetConfig("autoload") == "False") then
			atomsAddAutoload(this_package_name,this_package_version,allusers);
		end
		
		// Move the archive file (.tar.gz or .zip file) to the archive directory
		// =====================================================================
		
		stat = copyfile( fileout , archives_directory );
		
		if stat <> 1 then
			chdir(initialpath);
			error(msprintf(gettext("%s: Error while copying the file ''%s'' to the directory ''%s''.\n"),"atomsInstall",fileout,archives_directory));
		end
		
		mdelete( fileout );
		
		// Fill the result matrix
		// =====================================================================
		
		result = [ result ; atomsGetInstalledDetails(this_package_name,this_package_version) ];
		
		// Sucess message if needed
		// =====================================================================
		
		if VERBOSE then
			mprintf(" success\n");
		end
	end
	
	// Go to the initial location
	// =========================================================================
	chdir(initialpath);
	
endfunction


// =============================================================================
// Just get the list of the directories present in the current directory
// =============================================================================

function result = atomsListDir()
	
	result = [];
	
	items  = listfiles();
	
	// Loop on items
	for i=1:size(items,"*")
		if isdir(items(i)) then
			result = [ result ; items(i) ];
		end
	end
	
endfunction
