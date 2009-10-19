//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2009 - DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//
function loadScicosLibs()

  // list of scicos libraries that we need at xcos launch
  listlibsname = [];
  
  for theLib = scicos_pal_libs
    if isfile("SCI/modules/scicos_blocks/macros/" + theLib + "/lib") then
      load("SCI/modules/scicos_blocks/macros/" + theLib + "/lib");
      listlibsname = [listlibsname, theLib];
    end
  end
  clear theLib;

  if isfile('SCI/modules/scicos/macros/scicos_scicos/lib') then
    load('SCI/modules/scicos/macros/scicos_scicos/lib');
    listlibsname = [listlibsname, 'scicos_scicos'];
  end

  // name and libname 
  listlibsname(listlibsname == 'MatrixOp') = 'Matrixop';
  listlibsname(listlibsname == 'NonLinear') = 'Nonlinear';
  listlibsname(listlibsname == 'IntegerOp') = 'Integerop';
  
  if listlibsname <> [] then
    execline = '[' + strcat(listlibsname + 'lib',', ') + '] = resume(' + strcat(listlibsname + 'lib',', ')+ ');'
    execstr(execline);
  end

endfunction
