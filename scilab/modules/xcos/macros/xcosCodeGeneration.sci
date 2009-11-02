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
  import_from_hdf5(hdf5FileToLoad);
  
  [ok,XX,alreadyran,flgcdgen,szclkINTemp,freof] = ...
      do_compile_superblock42(scs_m, [], [], %f); 

  export_to_hdf5(hdf5FileToSave, "XX");
  
  xcosNotify(hdf5FileToSave);
endfunction