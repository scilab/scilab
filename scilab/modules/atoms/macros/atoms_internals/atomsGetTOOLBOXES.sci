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
	
	// Check input parameters
	// =========================================================================
	
	rhs  = argn(2);
	
	if rhs > 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: at most %d expected.\n"),"atomsGetTOOLBOXES",1));
	end
	
	if (rhs == 1) & (type(update) <> 4) then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsGetTOOLBOXES",1));
	end
	
	// If necessary, loop on available mirrors and download TOOLBOXES files
	
	if (fileinfo(pathconvert(SCIHOME+"/.atoms/packages",%F)) == []) | (rhs == 1 & update) then
		
		nb_TOOLBOXES = 0;
		TOOLBOXES    = []; // Liste des paths des fichiers TOOLBOXES
		
		mirrors = atomsRepositories();
		
		if ~isdir(pathconvert(TMPDIR+"/atoms")) then
			mkdir(pathconvert(TMPDIR+"/atoms"));
		end
		
		for i=1:size(mirrors,"*")
			
			url            = mirrors(i)+"/TOOLBOXES";
			file_out       = pathconvert(TMPDIR+"/atoms/"+sprintf("TOOLBOXES_%d",nb_TOOLBOXES),%f);
			
			if( fileinfo(file_out) <> [] ) then
				mdelete(file_out);
			end
			
			[rep,stat,err] = unix_g("wget "+url + " -O " + file_out)
			if stat == 0 then
				// Download successfull
				nb_TOOLBOXES = nb_TOOLBOXES + 1;
				TOOLBOXES    = [ TOOLBOXES ; file_out ];
			else
				disp("wget "+url + " -O " + file_out);
				disp(err);
			end
		end
		
		if nb_TOOLBOXES == 0 then
			error(msprintf(gettext("%s: Internal Error : No TOOLBOXES file has successfully been downloaded.\n"),"atomsGetTOOLBOXES"));
		end
		
		for i=1:size(TOOLBOXES,"*")
			packages = atomsReadTOOLBOXES( TOOLBOXES(i) , packages );
		end
		
		// Save the "packages" variable in a file
		
		if ~isdir(pathconvert(SCIHOME+"/.atoms")) then
			mkdir(pathconvert(SCIHOME+"/.atoms"));
		end
		
		save(pathconvert(SCIHOME+"/.atoms/packages",%F),packages)
		
	else
		
		// Just load from file
		load(pathconvert(SCIHOME+"/.atoms/packages",%F),"packages");

	end
	
endfunction
