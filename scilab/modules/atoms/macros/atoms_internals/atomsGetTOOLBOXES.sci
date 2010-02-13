// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

// Return the full packages of the TOOLBOXES present in the differents repositories

function packages = atomsGetTOOLBOXES(update)
	
	// Initialize
	packages = struct();
	
	// Operating system detection + Architecture detection
	// =========================================================================
	[OSNAME,ARCH,LINUX,MACOSX,SOLARIS,BSD] = atomsGetPlatform();
	
	// Check input parameters
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs > 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsGetTOOLBOXES",1));
	end
	
	if (rhs == 1) & (type(update) <> 4) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsGetTOOLBOXES",1));
	end
	
	// packages file path definition
	// =========================================================================
	
	packages_path      = atomsPath("system","user") + "packages";
	packages_path_info = fileinfo(packages_path);
	
	// If necessary, rebuild the struct
	// =========================================================================
	
	if (packages_path_info == []) ..
		| (getdate("s") - packages_path_info(6) > 3600) ..
		| (rhs == 1 & update) then
		
		// loop on available repositories
		// =====================================================================
		
		repositories = atomsRepositoryList();
		repositories = repositories(:,1);
		
		atoms_tmp_directory = atomsPath("system","session");
		
		if ~isdir(atoms_tmp_directory) then
			mkdir(atoms_tmp_directory);
		end
		
		for i=1:size(repositories,"*")
			
			// Building url & file_out
			// ----------------------------------------
			url            = repositories(i)+"/TOOLBOXES/"+ARCH+"/"+OSNAME+".gz";
			file_out       = pathconvert(atoms_tmp_directory+"TOOLBOXES.gz",%f);
			
			// Remove the existing file
			// ----------------------------------------
			if fileinfo(file_out) <> [] then
				mdelete(file_out);
			end
			
			// Launch the download
			// ----------------------------------------
			atomsDownload(url,file_out);
			
			// Extract It
			// ----------------------------------------
			
			if LINUX | MACOSX | SOLARIS | BSD then
				extract_cmd = "gunzip "+ file_out;
				
			else
				extract_cmd = getshortpathname(pathconvert(SCI+"/tools/gzip/gzip.exe",%F)) + " -d """ + file_out + """";
			end
			
			[rep,stat,err] = unix_g(extract_cmd);
			
			if stat ~= 0 then
				error(msprintf(gettext("%s: Extraction of the DESCRIPTION file (''%s'') has failed.\n"),"atomsGetTOOLBOXES",file_out));
			end
			
			file_out = strsubst(file_out,"/\.gz$/","","r");
			
			// Read the download description file
			// ----------------------------------------
			additional("repository") = repositories(i);
			this_description         = atomsDESCRIPTIONread(file_out,additional);
			
			// Add information about the repository
			// ----------------------------------------
			this_description =  atomsDESCRIPTIONaddField(this_description,"*","*","repository",repositories(i));
			this_description =  atomsDESCRIPTIONaddField(this_description,"*","*","fromRepository","1");
			
			// concatenate the description with the 
			// global struct
			// ----------------------------------------
			packages = atomsDESCRIPTIONcat( packages , this_description );
			
			// file_out is no more needed
			// ----------------------------------------
			mdelete(file_out);
			
		end
		
		// Get DESCRIPTIONs for "archive" installation
		// =====================================================================
		
		description_files = [ ..
			atomsPath("system","allusers") + "DESCRIPTION_archives" ; ..
			atomsPath("system","user")     + "DESCRIPTION_archives" ; ..
			atomsPath("system","session")  + "DESCRIPTION_archives" ];
		
		for i=1:size(description_files,"*")
			if ~ isempty(fileinfo(description_files(i))) then
				description = atomsDESCRIPTIONread(description_files(i));
				packages    = atomsDESCRIPTIONcat( packages , description );
			end
		end
		
		// Save the "packages" variable in a file
		// =====================================================================
		if ~isdir(atomsPath("system","user")) then
			mkdir(atomsPath("system","user"));
		end
		
		save(packages_path,packages)
	
	// Just load from file
	// =========================================================================
	
	else
		load(packages_path,"packages");
	end
	
endfunction
