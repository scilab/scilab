//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Bruno JOFRET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

function xcosCodeGeneration(hdf5FileToLoad, hdf5FileToSave)
  
//-- BJ : Alias Warning Function
  prot = funcprot();
  funcprot(0);
  hilite_obj = xcosShowBlockWarning;
  unhilite_obj = xcosClearBlockWarning;
  funcprot(prot);
  //-- end
  
  // This will create a scs_m variable.
  status = import_from_hdf5(hdf5FileToLoad);
  if ~status then
    error(msprintf(gettext("%s: Unable to import data from %s"), "xcosCodeGeneration", hdf5FileToLoad));
  end
  
  ierr = execstr("[ok] = do_compile_superblock42(scs_m, [], [], %f); ", 'errcatch');
  if ierr <> 0 then
	  [msg, err] = lasterror();
	  disp(msg);
	  deletefile(hdf5FileToSave);
	  return;
  end
  
  if ok then
    status = export_to_hdf5(hdf5FileToSave, "XX");
    if ~status then
      error(msprintf(gettext("%s: Unable to export ''XX'' to %s"), "xcosCodeGeneration", hdf5FileToSave));
    end
  else
    deletefile(hdf5FileToSave);
  end
endfunction

