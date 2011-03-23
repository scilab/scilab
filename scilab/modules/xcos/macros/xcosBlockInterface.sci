//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
// Copyright (C) 2009-2010 - DIGITEO - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function xcosBlockInterface(hdf5FileToLoad, hdf5FileToSave, ...
			    interfaceAlias, job, hdf5ContextFile)
	
	// push some old scicos variable in environment. 
	needcompile = 0;
	alreadyran = %f;
	
	// define context
	import_from_hdf5(hdf5ContextFile);
	%scicos_context = struct();
	[%scicos_context, ierr] = script2var(context, %scicos_context)
	
	//for backward compatibility for scifunc
	if ierr==0 then
		%mm = getfield(1,%scicos_context)
		for %mi=%mm(3:$)
			ierr = execstr(%mi+'=%scicos_context(%mi)','errcatch')
			if ierr<>0 then
				break; //** in case of error exit 
			end
		end
	end
	//end of for backward compatibility for scifunc

	status = import_from_hdf5(hdf5FileToLoad);
	if ~status then
		error(msprintf(gettext("%s: Unable to import data from %s"), "xcosBlockInterface", hdf5FileToLoad));
	end
	
	ierr = execstr("[new_scs_m, y, typ] = interfaceAlias(job, scs_m, [])", 'errcatch');
	if ierr <> 0 then
		[msg, err] = lasterror();
		disp(msg);
		export_to_hdf5(hdf5FileToSave, "scs_m");
		return;
	end
	
	// Check if the block has been updated or not.
	// If the data has changed then we don't need to recompile (indicated by 
	// no file creation).
	updated = and([needcompile == 0, and(new_scs_m == scs_m)]) <> %t;
	if updated then
		export_to_hdf5(hdf5FileToSave, "new_scs_m");
	else
		deletefile(hdf5FileToSave);
	end

endfunction

