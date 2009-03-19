// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2008 - INRIA - Delphine GASC <delphine.gasc@scilab.org>
// Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU <sylvestre.ledru@scilab.org>
// Copyright (C) 2009 - DIGITEO - Pierre MARECHAL <pierre.marechal@scilab.org>
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

// End user function

// Display of the toolbox information

function atomsInfo(name)

	rhs=argn(2);
	
	if rhs <> 1 then
		error(msprintf(gettext("%s: Wrong number of input argument: %d expected.\n"),"atomsInfo",1));
	end
	
	// we remove the special characters
	normalized_name = atomsSubstituteString(name)
	
	// We try to find the information locally
	// =========================================================================
	atoms_directories        = atomsToolboxDirectory();
	this_toolbox_directory   = "";
	this_toolbox_DESCRIPTION = "";
	
	for k=1:size(atoms_directories,"*")
		check_directory = pathconvert(atoms_directories(k)+"/"+normalized_name);
		check_file      = check_directory+"DESCRIPTION";
		if fileinfo(check_file) <> [] then
			this_toolbox_directory   = check_directory;
			this_toolbox_DESCRIPTION = check_file;
			break;
		end
	end
	
	if this_toolbox_DESCRIPTION <> "" then
		desc = atomsReadDesc(nameToolbox);
		disp(gettext("This toolbox is present locally."));
		disp(desc);
		disp(functionTool);
		
		// Ok, we have done the job : return
		return;
	end
	
	// We try to find the information on the net
	// =========================================================================
	
	disp(_("Search for toolbox versions available on the web"))
	
	listDesc = atomsReadDesc("");
	versions = "";
	[n, m]   = size(listDesc("Toolbox"));
	
	for i=1:n
		if listDesc("Toolbox")(i) == nameToolbox
			// To avoid version redundancy
			[a, b] = size(versions)
			
			if find(versions == listDesc("Version")(i))
				continue;
			end
			
			versions(a+1)        = listDesc("Version")(i)
			desc                 = atomsListDescription()
			[listeObl, listeOpt] = constant()
			[o, p]               = size(listeOpt)
			[n, m]               = size(listeObl)
			for j=1:m
				desc(listeObl(j)) = listDesc(listeObl(j))(i)
			end
			for j=1:p
				desc(listeOpt(j)) = listDesc(listeOpt(j))(i)
			end
			
			disp(desc);
		end
	end
	
	// if no version was found
	// =========================================================================
	[a, b] = size(versions)
	if a == 1 then
		disp("none")
	end

endfunction
