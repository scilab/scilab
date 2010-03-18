//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
//import xcos diagram in Scilab environment
//parameters : 
//xcosFile : xcos diagram file
//result : boolean


function result = importXcosDiagram(xcosFile)

	result = %f;
	rhs = argn(2);
	
	if(rhs == 1) then 
		//save hdf5 file with xcos filename, just change extension
		[path,fname,extension] = fileparts(xcosFile);
		h5File = TMPDIR + filesep() + fname + ".h5";
		// open the file to check for permissions
		[a, err] = mopen(xcosFile, "r");
		if (err <> 0) then
			error(msprintf(gettext("Unable to open %s"), xcosFile));
			return;
		end
		// construct a full path string
		fullPathName = get_absolute_file_path(fname + extension) + fname + extension;
		mclose(a);
	else
		error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "importXcosDiagram", 1));
		return;
	end
	
	convertStatus = xcosDiagramToHDF5(fullPathName, h5File, %t);
	if(convertStatus <> 0) then
		error(msprintf(gettext("%s: Unable to import xcos file ""%s"".\n"), "importXcosDiagram", xcosFile));
	end
	
	if(import_from_hdf5(h5File) == %f) then
		error(msprintf(gettext("%s: Unable to import xcos file ""%s"".\n"), "importXcosDiagram", xcosFile));
		return;
	end

	// deleting the temporary file
	mdelete(h5File);
	
	//return scs_m in Scilab environment
	result = %t;
	scs_m = resume(scs_m);
endfunction


