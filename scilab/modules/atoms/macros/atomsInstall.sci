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

function result = atomsInstall(packages,section)
	
	// Load Atoms Internals lib if it's not already loaded
	// =========================================================================
	if ~ exists("atomsinternalslib") then
		load("SCI/packages/atoms/macros/atoms_internals/lib");
	end
	
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
	
	if size(packages(1,:),"*") > 2 then
		error(msprintf(gettext("%s: Wrong size for input argument #%d: mx1 or mx2 string matrix expected.\n"),"atomsInstall",1));
	end
	
	// Remove leading and trailing whitespace
	// =========================================================================
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
	
	// Architecture detection
	// =========================================================================
	
	[dynamic_info,static_info] = getdebuginfo();
	arch_info  = static_info(grep(static_info,"/^Compiler Architecture:/","r"))
	
	if ~isempty(arch_info) & (regexp(arch_info,"/\sX64$/","o") <> []) then
		ARCH = "64";
	else
		ARCH = "32";
	end
	
	// Verbose Mode ?
	// =========================================================================
	if strcmpi(atomsGetConfig("Verbose"),"True") == 0 then
		ATOMSVERBOSE = %T;
	else
		ATOMSVERBOSE = %F;
	end
	
	// Allusers/user management
	//   - If Allusers is equal to "allusers", packages will installed in the "allusers" section :
	//       → SCI/contrib    : location of the packages
	//       → SCI/.atoms     : ATOMS system files
	//   - Otherwise, packages will installed in the "user" section :
	//       → SCIHOME/atoms  : location of the packages
	//       → SCIHOME/.atoms : location of the packages & ATOMS system files
	// =========================================================================
	
	if rhs <= 1 then
		
		// By default: 
		//  → Install in the "allusers" section if we have the write access to
		//    SCI directory
		//  → Install in the "user" otherwise
		
		if atomsAUWriteAccess() then
			section = "allusers"; 
		else
			section = "user";
		end
		
	else
		
		// Process the 2nd input argument : section
		// Allusers can be a boolean or equal to "user" or "allusers"
		
		if type(section) <> 10 then
			chdir(initialpath);
			error(msprintf(gettext("%s: Wrong type for input argument #%d: A single-string expected.\n"),"atomsInstall",2));
		end
		
		if and(section<>["user","allusers"]) then
			chdir(initialpath);
			error(msprintf(gettext("%s: Wrong value for input argument #%d: ''user'' or ''allusers'' expected.\n"),"atomsInstall",2));
		end
		
		// Check if we have the write access
		if (section=="allusers") & ~ atomsAUWriteAccess() then
			chdir(initialpath);
			error(msprintf(gettext("%s: You haven''t write access on this directory : %s.\n"),"atomsInstall",2,pathconvert(SCI+"/.atoms")));
		end
	end
	
	// Create needed directories
	// =========================================================================
	atoms_system_directory  = atomsPath("system" ,section);
	atoms_install_directory = atomsPath("install",section);
	atoms_tmp_directory     = atomsPath("system" ,"session");
	
	if ~ isdir( atoms_system_directory ) & (mkdir( atoms_system_directory ) <> 1) then
		error(msprintf( ..
			gettext("%s: The directory ''%s'' cannot been created, please check if you have write access on this directory.\n"),..
			atoms_system_directory));
	end
	
	if ~ isdir( atoms_install_directory ) & (mkdir( atoms_install_directory ) <> 1) then
		error(msprintf( ..
			gettext("%s: The directory ''%s'' cannot been created, please check if you have write access on this directory.\n"),..
			atoms_install_directory));
	end
	
	if ~ isdir(atoms_tmp_directory) & (mkdir(atoms_tmp_directory) <> 1) then
		error(msprintf( ..
			gettext("%s: The directory ''%s'' cannot been created, please check if you have write access on this directory.\n"),..
			atoms_tmp_directory));
	end
	
	// Define the "archives" directory path
	// Create it if it's not exist
	// =========================================================================
	archives_directory = atoms_install_directory + "archives";
	
	if ~ isdir( archives_directory ) & (mkdir( archives_directory ) <> 1) then
		error(msprintf( ..
			gettext("%s: The directory ''%s'' cannot been created, please check if you have write access on this directory.\n"),..
			"atomsInstall", ..
			archives_directory));
	end
	
	// "Archive" installation
	// =========================================================================
	
	for i=1:size(packages(:,1),"*")
		
		this_package = packages(i,1);
		
		if ~ isempty(regexp(this_package,"/(\.tar\.gz|\.tgz|\.zip)$/","o")) then
			
			if fileinfo( this_package ) then
				error(msprintf(gettext("%s: The file ''%s'' doesn''t exist or is not read accessible\n"),"atomsInstall",this_package));
			end
			
			tmp_dir = atomsExtract(this_package,atoms_tmp_directory);
			tmp_dir = pathconvert(atoms_tmp_directory+tmp_dir);
			
			if fileinfo( tmp_dir + "DESCRIPTION" ) then
				error(msprintf(gettext("%s: DESCRIPTION file cannot be found in the package ''%s''\n"),"atomsInstall",this_package));
			end
			
			this_package_description = atomsDESCRIPTIONread(tmp_dir + "DESCRIPTION");
			
			// Get package name and version
			// -----------------------------------------------------------------
			
			this_package_name    = getfield(1,this_package_description);
			this_package_name    = this_package_name(3);
			
			this_package_version = getfield(1,this_package_description(this_package_name));
			this_package_version = this_package_version(3);
			
			// Save the extracted directory
			// -----------------------------------------------------------------
			
			this_package_description = atomsDESCRIPTIONaddField( .. 
				this_package_description, ..
				this_package_name,        ..
				this_package_version,     ..
				"extractedDirectory",     ..
				tmp_dir);
			
			this_package_description = atomsDESCRIPTIONaddField( .. 
				this_package_description, ..
				this_package_name,        ..
				this_package_version,     ..
				"archiveFile",            ..
				this_package);
				
			this_package_description = atomsDESCRIPTIONaddField( .. 
				this_package_description, ..
				this_package_name,        ..
				this_package_version,     ..
				"fromRepository",         ..
				"0");
			
			// Save the DESCRIPTION_archives
			// -----------------------------------------------------------------
			
			if fileinfo( atoms_tmp_directory + "DESCRIPTION_archives" )<>[] then
				packages_description = atomsDESCRIPTIONread(atoms_tmp_directory+"DESCRIPTION_archives");
				packages_description = atomsDESCRIPTIONcat(packages_description,this_package_description);
			else
				packages_description = this_package_description;
			end
			
			atomsDESCRIPTIONwrite(packages_description,atoms_tmp_directory+"DESCRIPTION_archives");
			
			// change the packages var
			// -----------------------------------------------------------------
			packages(i,:) = [ this_package_name this_package_version ];
			
		end
		
	end
	
	// Force update the system informations
	// =========================================================================
	atomsGetTOOLBOXES(%T)
	
	// Get the install list
	// =========================================================================
	[install_package_list,dependency_tree] = atomsInstallList(packages,section);
	
	// Loop on install_package_list to print if a package has to be installed
	// or not
	// =========================================================================
	for i=1:size(install_package_list(:,1),"*")
		if install_package_list(i,1) == "+" then
			atomsDisp(msprintf("\t%s (%s) will be installed\n\n",install_package_list(i,3),install_package_list(i,4)));
		elseif install_package_list(i,1) == "~" then
			atomsDisp(msprintf("\t%s (%s) is already installed and up-to-date\n\n",install_package_list(i,3),install_package_list(i,4)));
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
		
		atomsDisp(msprintf("\tInstalling %s (%s) ...",this_package_name,this_package_version));
		
		// Define the path of the directory where will be installed this toolbox
		// =====================================================================
		this_package_directory = atomsPath("install",section) + this_package_name + filesep();
		
		// Create the parent directory of this toolbox if it's not already exist
		// =====================================================================
		
		if ~isdir(this_package_directory) & (mkdir(this_package_directory)<>1) then
			chdir(initialpath);
			error(msprintf( ..
				gettext("%s: The directory ""%s"" cannot been created, please check if you have write access on this directory.\n"),..
				this_package_directory));
		end
		
		// "Repository" installation ; Download and Extract
		// =====================================================================
		
		if this_package_details("fromRepository") == "1" then
			
			// Define the path of the downloaded file
			// =================================================================
			
			if isfield(this_package_details,OSNAME+ARCH+"Name") then
				OSTYPE = OSNAME+ARCH;
			else
				OSTYPE = OSNAME;
			end
			
			fileout = pathconvert(this_package_directory+this_package_details(OSTYPE+"Name"),%F);
			filein  = this_package_details(OSTYPE+"Url");
			filemd5 = this_package_details(OSTYPE+"Md5");
			
			// Launch the download
			// =================================================================
			atomsDownload(filein,fileout,filemd5);
			
			// unarchive it
			// =================================================================
			this_package_details("extractedDirectory") = this_package_directory + atomsExtract(fileout,this_package_directory);
		end
		
		// Rename the created directory
		// =====================================================================
		
		if MSDOS then
			rename_cmd = "rename """+this_package_details("extractedDirectory")+""" """+this_package_version+"""";
		else
			rename_cmd = "mv """+this_package_details("extractedDirectory")+""" """+this_package_directory+this_package_version+"""";
		end
		
		[rep,stat]=unix_g(rename_cmd)
		
		if stat <> 0 then
			disp(rename_cmd);
			chdir(initialpath);
			error(msprintf(gettext("%s: Error while creating the directory ''%s''.\n"),"atomsInstall",pathconvert(this_package_directory+this_package_version)));
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
		
		atomsInstallRegister(this_package_name,this_package_version,this_package_status,section);
		
		// Autoload the toolbox unless precised
		// =====================================================================
		
		if ~ (atomsGetConfig("autoload") == "False") then
			// Add a package to the autoload list only if it's intentionnaly
			// installed
			if this_package_status=="I" then
				atomsAutoloadAdd(this_package_name,this_package_version,section);
			end
		end
		
		// Move the archive file (.tar.gz or .zip file) to the archive directory
		// =====================================================================
		
		if this_package_details("fromRepository")=="1" then
			this_package_archive = fileout;
		else
			this_package_archive = this_package_details("archiveFile");
		end
		
		if copyfile( this_package_archive , archives_directory ) <> 1 then
			chdir(initialpath);
			error(msprintf(gettext("%s: Error while copying the file ''%s'' to the directory ''%s''.\n"),"atomsInstall",this_package_archive,archives_directory));
		end
		
		if this_package_details("fromRepository")=="1" then
			mdelete( fileout );
		end
		
		// Fill the result matrix
		// =====================================================================
		result = [ result ; atomsGetInstalledDetails([this_package_name this_package_version]) ];
		
		// "archive" installation : Save the description
		// =====================================================================
		
		if this_package_details("fromRepository")=="0" then
			
			DESCRIPTION_file = atoms_directory+"DESCRIPTION_archives";
			
			if isempty(fileinfo(atoms_directory+"DESCRIPTION_archives")) then
				DESCRIPTION = struct();
			else
				DESCRIPTION = atomsDESCRIPTIONread(DESCRIPTION_file);
			end
			
			DESCRIPTION = atomsDESCRIPTIONadd(DESCRIPTION,this_package_name,this_package_version,this_package_details);
			atomsDESCRIPTIONwrite(DESCRIPTION,DESCRIPTION_file);
			
		end
		
		// Sucess message if needed
		// =====================================================================
		
		atomsDisp(msprintf(" success\n\n"));
		
	end
	
	// The TMPDIR DESCRIPTION_archives is no more needed
	// =========================================================================
	
	if ~ isempty(fileinfo(atoms_tmp_directory + "DESCRIPTION_archives")) then
		mdelete(atoms_tmp_directory + "DESCRIPTION_archives");
	end
	
	// Update the dependencies of packages that use another version of packages
	// that have been installed
	// =========================================================================
	
	for i=1:size( result(:,1) , "*" )
		
		packages_out = atomsUpdateDeps([result(i,1) result(i,2)],section);
		
		if ATOMSVERBOSE then
			for j=1:size(packages_out(:,1),"*")
				atomsDisp(msprintf("\t%s (%s) will now use the version %s of the package %s\n\n",packages_out(j,1),packages_out(j,2),result(i,1),result(i,2)));
			end
		end
		
	end
	
	// Remove orphan packages
	// =========================================================================
	
	orphan_list = atomsOrphanList(section);
	if ~ isempty(orphan_list) then
		atomsRemove( [ orphan_list(:,1) orphan_list(:,2) ] );
	end
	
	// Go to the initial location
	// =========================================================================
	chdir(initialpath);
	
endfunction
