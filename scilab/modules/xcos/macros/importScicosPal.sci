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


//load all files
varsToLoad = [];
for fIndex = 1:size(palFiles, "*")

  if ~isfile(palFiles(fIndex)) then
    error(msprintf(gettext("%s: File ''%s'' does not exist.\n"), "importScicosPal", palFiles(fIndex)));
    return
  end
  
  exec(palFiles(fIndex), -1);
  names = who("get");

  // Try to find scs_m variables
  for kVar = 1:size(names, "*")
    if ~isempty(strindex(names(kVar), "scs_m")) then
      currentPal = eval(names(kVar));
      for kBlock = 1:size(currentPal.objs)
	if typeof(currentPal.objs(kBlock))=="Block" then
	  if find(varsToLoad == currentPal.objs(kBlock).gui) == [] then
	    varsToLoad($+1) = currentPal.objs(kBlock).gui;
	  end
	end
      end
    end
  end
end

for kBlock = 1 : size(varsToLoad, "*")

  BlockFile = outPath + varsToLoad(kBlock) + ".h5";

  ierr = execstr("out  = " + varsToLoad(kBlock) + "(""define"")", "errcatch");
  
  if ierr == 0 then
    mprintf("%d: %s\n",  kBlock, varsToLoad(kBlock));
    bexport = export_to_hdf5(BlockFile, "out");
    if (~bexport) then
      mprintf(gettext("FAILED TO EXPORT: %s\n"),  varsToLoad(kBlock));
    end
    out1 = out;
    bImport = import_from_hdf5(BlockFile);
    if bImport == %f | or(out1 ~= out) then
      mprintf(gettext("FAILED TO EXPORT: %s\n"),  varsToLoad(kBlock));
    end
  end
  
end
endfunction
