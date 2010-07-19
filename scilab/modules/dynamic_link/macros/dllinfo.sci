// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function dllinfolist = dllinfo(dllname, options)

  dllinfolist = list();
  if getos() <> 'Windows' then
    warning(msprintf(gettext("%s: This feature has been implemented for Windows.\n"),'dllinfo'));
  else
    // Load dynamic_link Internal lib if it's not already loaded
    if ~ exists("dynamic_linkwindowslib") then
      load("SCI/modules/dynamic_link/macros/windows/lib");
    end

    [lhs,rhs] = argn(0);

    if rhs == 1 then
      options = 'exports';
    end

    if type(dllname) <> 10 | size(dllname, "*") <> 1  then
      error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),'dllinfo',1));
    end

    if fileinfo(dllname) == [] then
      error(msprintf(gettext("%s: The file %s does not exist.\n"),'dllinfo',dllname));
    end

    fext = fileext(dllname);
    if ~(strcmpi(fext,'.exe') == 0 | strcmpi(fext,'.dll')== 0) then
      error(msprintf(gettext("%s: Cannot open file %s.\n"),'dllinfo',dllname));
    end

    if type(options) <> 10 | size(options) <> 1 then
      error(msprintf(gettext("%s: Wrong type for input argument #%d: A string expected.\n"),'dllinfo',2));
    end

    if ~(strcmpi(options,'imports')== 0 | strcmpi(options,'exports')== 0 | strcmpi(options,'machine')== 0) then
      error(msprintf(gettext("%s: Wrong value for input argument #%d: Must be in the set {%s}.\n"),'dllinfo',2,'''imports'',''exports'',''machine'''));
    end

    dllinfolist = dlwDllInfo(dllname, options);
  end
endfunction
//=============================================================================
