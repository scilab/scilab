//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
function importScicosPal(palFiles, outPath)

rhs = argn(2);

if rhs < 2 then
  error(msprintf(gettext("%s: Wrong number of input argument(s): %d expected.\n"), "importScicosPal", 2));
  return
end

for fIndex = 1:size(palFiles, "*")

  if ~isfile(palFiles(fIndex)) then
    error(msprintf(gettext("%s: File ''%s'' does not exist.\n"), "importScicosPal", palFiles(fIndex)));
    return
  end
  
  exec(palFiles(fIndex), -1);
  names = who("get");

  // Try to find scs_m variables
  varsToLoad = [];
  for kVar = 1:size(names, "*")
    if ~isempty(strindex(names(kVar), "scs_m")) then
      varsToLoad($+1) = names(kVar);
    end
  end

  for kVar = 1:size(varsToLoad, "*")

    currentPal = eval(varsToLoad(kVar));

    for kBlock = 1:size(currentPal.objs)

      if typeof(currentPal.objs(kBlock))=="Block" then
	
	ierr = execstr("out  = " + currentPal.objs(kBlock).gui + "(""define"")", "errcatch");
	
	if ierr==0 then
	  mprintf("%s\n",  currentPal.objs(kBlock).gui);
	  //export_to_hdf5(outPath + filesep() + currentPal.objs(kBlock).gui + ".h5", "out");
	end
	
      end
    end
    
  end
  
end

endfunction
