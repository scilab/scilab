
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function [make_command, lib_name_make, lib_name, path, makename, files] = ..
             dlwGetParamsIlibCompil(lib_name, makename, files)

  managed_ext = ['.obj', '.o'];
  for i=1:size(files,'*') // compatibility scilab 4.x
    [path_f, file_f, ext_f] = fileparts(files(i));
    if or(managed_ext == ext_f) then
      files(i) = path_f + file_f;
    else
      files(i) = path_f + file_f + ext_f;
    end
  end

  k = strindex(makename,['/','\']);

  if k~=[] then
    path = part(makename,1:k($));
    makename = part(makename,k($)+1:length(makename));
  else
     path='';
  end

  lib_name = lib_name+getdynlibext();
  lib_name_make = lib_name;

  FILES = [];
  for x = files(:)'
    [ptmp,ftmp,fext] = fileparts(x);
    FILES = [FILES,ptmp+ftmp];
  end

  if (makename <> [] & makename <> '') then
    makename = makename + dlwGetMakefileExt() ;
  else
    makename = dlwGetDefltMakefileName() + dlwGetMakefileExt() ;
  end

  make_command = 'nmake /Y /nologo /f ';

  if FILES <> [] then
    files = FILES + '.obj' ;
  end

endfunction
//=============================================================================
