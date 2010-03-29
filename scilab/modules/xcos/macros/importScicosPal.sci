//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Vincent COUVERT
// Copyright (C) 2009 - DIGITEO - Antoine ELIAS
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
  out1 = scicos_diagram();
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
            out1.objs($ + 1) = currentPal.objs(kBlock);
          end
        end  
      end
    end //for kVar
  end //for fIndex
  
  //check and update scicos block version
  out1 = importScicosDiagram(out1);
  
  exportedBlocks = 0;
  for i = 1:size(out1.objs)
    out = out1.objs(i);
    block_name = out.gui + ".h5";
    blockFile = outPath + block_name
    
    doExport = %t;
    if isfile(blockFile) then
      //if the file already exists try to load data and compare
      out2 = out;
      bImport = import_from_hdf5(blockFile);
      
      //data are identical
      if bImport == %t & out == out2 then
        doExport = %f;
      end
    end
    
    if doExport == %t then
      mprintf("%d: %s\n",  i, block_name);
      bexport = export_to_hdf5(blockFile, "out");
      if (~bexport) then
        mprintf(gettext("FAILED TO EXPORT: %s\n"), out.gui);
      end
    
      out2 = out;
      bImport = import_from_hdf5(blockFile);
      
      if bImport == %f | or(out2 <> out) then
        mprintf(gettext("FAILED TO EXPORT: %s\n"), out.gui);
      end
      exportedBlocks = exportedBlocks + 1;
    end
  end
  disp("exportedBlocks : " + string(exportedBlocks));
  
endfunction

