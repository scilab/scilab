// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) DIGITEO - 2009-2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function bOK = configure_msvc()

  bOK = %F;
  if getos() <> 'Windows' then
    return
  end

  // Load dynamic_link Internal lib if it's not already loaded
  if ~ exists("dynamic_linkwindowslib") then
    load("SCI/modules/dynamic_link/macros/windows/lib");
  end

  msvc = findmsvccompiler();
  if win64() & detectmsvc64tools() then
    bOK = dlwConfigureVcx64();
  else
    bOK = dlwConfigureVcx86();
    if ~bOK then
      warning(msprintf(gettext('Microsoft Visual Studio C x86 Compiler not found.')));
    end
  end


endfunction
//=============================================================================

