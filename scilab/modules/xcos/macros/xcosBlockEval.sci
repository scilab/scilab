//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009-2009 - DIGITEO - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//

//hdf5FileToLoad : input block file
//hdf5FileToSave : output block file
//interfaceAlias : block interface function name ( ex "IN_f" )
//hdf5ContextFile : input context file

function xcosBlockEval(hdf5FileToLoad, hdf5FileToSave, interfaceAlias, hdf5ContextFile)

	//replace scicos_getvalue by	setvalue ( call by interfaceAlias )
  %mprt = funcprot()
  funcprot(0)
  scicos_getvalue = setvalue;
  function message(txt)
    messagebox(['In block ' + o.gui + ': ';
		txt;
	       'current parameter value kept'],'error','modal');
    [str,n,line,func]=lasterror();
    printf('do_eval: error %d - %s in %s at line %d\n', n, str, func, line); 
  endfunction
  %scicos_prob = %f
  funcprot(%mprt)

	import_from_hdf5(hdf5ContextFile);
	%scicos_context = struct();
	[%scicos_context, ierr] = script2var(context, %scicos_context)

	import_from_hdf5(hdf5FileToLoad);


  //create a structure with the new context
  [new_scs_m, y, typ] = interfaceAlias('set', scs_m, []);

	export_to_hdf5(hdf5FileToSave, "new_scs_m");
endfunction
