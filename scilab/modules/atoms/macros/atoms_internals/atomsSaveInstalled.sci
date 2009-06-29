// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// Internal function

function  atomsSaveInstalled(installed_struct,allusers)
	
	rhs = argn(2);
	
	// Check number of input arguments
	// =========================================================================
	
	if rhs <> 2 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsSaveInstalled",2));
	end
	
	// Check type of input argument type
	// =========================================================================
	
	if type(installed_struct) <> 17 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A struct expected.\n"),"atomsSaveInstalled",1));
	end
	
	if type(allusers) <> 4 then
		error(msprintf(gettext("%s: Wrong type for input argument #%d: A boolean expected.\n"),"atomsSaveInstalled",2));
	end
	
	// Define the path of the file that will record the change according to
	// the "allusers" value
	// =========================================================================
	
	if allusers then
		atoms_directory = pathconvert(SCI+"/.atoms");
	else
		atoms_directory = pathconvert(SCIHOME+"/atoms");
	end
	
	// Does the atoms_directory exist, if not create it
	// =========================================================================
	
	if ~ isdir(atoms_directory) then
		mkdir(atoms_directory);
	end
	
	// Define the path of the file that will record the change according 
	// =========================================================================
	installed_txt = atoms_directory+"installed.txt";
	installed_bin = atoms_directory+"installed.bin";
	
	// Build the installed_mat and the installed_str
	// =========================================================================
	
	installed_mat = [];
	installed_str = [];
	
	packages      = getfield(1,installed_struct);
	packages(1:2) = [];
	
	for i=1:size(packages,"*")
		
		this_package_details  = installed_struct(packages(i));
		
		this_package_name     = this_package_details(1);
		this_package_version  = this_package_details(2);
		this_package_path     = this_package_details(3);
		this_package_allusers = this_package_details(4);
		this_package_status   = this_package_details(5);
		
		installed_mat = [ installed_mat ; this_package_name this_package_version this_package_allusers this_package_path this_package_status];
		installed_str = [ installed_str ; "[ "+this_package_name+" - "+this_package_version+" - "+this_package_allusers+" - "+this_package_path+" - "+this_package_status+" ]" ];
		
	end
	
	// If packages is empty, no need to keep the files
	// =========================================================================
	
	if isempty(packages) then
		mdelete(installed_txt);
		mdelete(installed_bin);
		return;
	end
	
	// write files
	// =========================================================================
	
	mputl( installed_str , installed_txt);
	save( installed_bin , installed_struct , installed_mat );
	
endfunction
