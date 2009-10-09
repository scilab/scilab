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

function xcosBlockInterface(hdf5FileToLoad, hdf5FileToSave, ...
			    interfaceAlias, job, hdf5ContextFile)
// disp("hdf5FileToLoad = "+ hdf5FileToLoad)
// disp("hdf5FileToSave = "+ hdf5FileToSave)
  
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
  
  import_from_hdf5(hdf5FileToLoad);

  [new_scs_m, y, typ] = interfaceAlias(job, scs_m, []);
  export_to_hdf5(hdf5FileToSave, "new_scs_m");
  xcosNotify(hdf5FileToSave);
endfunction
