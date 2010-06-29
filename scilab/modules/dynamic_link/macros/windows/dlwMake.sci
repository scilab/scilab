// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function res = dlwMake(files, objects_or_dll)

  if ~haveacompiler() then
    error(msprintf(gettext('%s: A Fortran or C compiler is required.\n'), 'G_make'))
  end

  [lhs,rhs] = argn(0);
  if rhs <> 2 then
    error(msprintf(gettext("%s: Wrong number of input argument(s).\n"), "G_make"));
  end

  msg = '';

  if typeof(objects_or_dll)<>'string' then
    error(sprintf(gettext('%s: Wrong type for input argument #%d: String expected.'), 'G_make', 2));
  end

  OBJ = '';
  [ptmp, ftmp, fext] = fileparts(objects_or_dll);
  OBJ = ptmp + ftmp;

  // scilab uses Visual Studio C++
  if ilib_verbose() > 1 then
    msg = unix_g('nmake /Y /nologo /f Makefile.mak ' + OBJ);
    disp(msg);
  else
    host('nmake /Y /nologo /f Makefile.mak ' + OBJ);
  end

  res = [OBJ];

endfunction
//=============================================================================
