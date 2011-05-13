//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
// Copyright (C) 2011 - DIGITEO - Clément DAVID
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
function loadXcosLibs()

  if ~isdef('scicos_menuslib') then
    load('SCI/modules/scicos/macros/scicos_menus/lib')
  end
 

  if ( ~isdef("scicos_pal") | ~isdef("%scicos_menu") | ..
       ~isdef("%scicos_short") | ~isdef("%scicos_help") | ..
       ~isdef("%scicos_display_mode") | ~isdef("modelica_libs") | ..
       ~isdef("scicos_pal_libs") ) then

    
     [scicos_pal, %scicos_menu, %scicos_short, modelica_libs, scicos_pal_libs,...
     %scicos_lhb_list, %CmenuTypeOneVector, %scicos_gif,%scicos_contrib, ..
     %scicos_libs, %scicos_with_grid, %scs_wgrid] = initial_scicos_tables();

    clear initial_scicos_tables
  end

  // list of scicos libraries that we need at xcos launch
  listlibsname = [];
  
  for theLib = scicos_pal_libs
    if isfile("SCI/modules/scicos_blocks/macros/" + theLib + "/lib") then
      load("SCI/modules/scicos_blocks/macros/" + theLib + "/lib");
      listlibsname = [listlibsname, theLib];
    end
  end
  clear theLib;

  if isfile('SCI/modules/scicos/macros/scicos_menus/lib') then
    load('SCI/modules/scicos/macros/scicos_menus/lib');
    listlibsname = [listlibsname, 'scicos_menus'];
  end

  if isfile('SCI/modules/scicos/macros/scicos_scicos/lib') then
    load('SCI/modules/scicos/macros/scicos_scicos/lib');
    listlibsname = [listlibsname, 'scicos_scicos'];
  end

  // name and libname 
  listlibsname(listlibsname == 'MatrixOp') = 'Matrixop';
  listlibsname(listlibsname == 'NonLinear') = 'Nonlinear';
  listlibsname(listlibsname == 'IntegerOp') = 'Integerop';
  
  if listlibsname <> [] then
    resumedLibs = listlibsname + 'lib';
  else
    resumedLibs = string([]);
  end

  // Compatibility interface functions
  function [x,y,typ]=COMPAT_BLOCK(job,arg1,arg2)
  // Throw an error on block access
      x=[];y=[];typ=[]
      if ~exists("arg1") then
          arg1 = mlist(['Block', "gui"], "COMPAT_BLOCK");
      end
      error(msprintf(gettext("%s: the block ""%s"" is no more available, please update the diagram with a compatible one."), "loadXcosLibs", arg1.gui));
  endfunction

  // removed blocks
  removed = [
"AFFICH_f"
"RFILE"
"WFILE"];
  prot = funcprot();
  funcprot(0);
  execstr(strcat(removed + "=COMPAT_BLOCK; "));
  funcprot(prot);
  
  resumedBlocks = removed';

  // put all resumed symbols into the parent scope
  prot = funcprot();
  execstr("funcprot(0); [" + strcat([resumedLibs resumedBlocks], ", ") + "] = resume(" + strcat([resumedLibs resumedBlocks], ", ") + "); funcprot(" + string(prot) + ");");
endfunction

