// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2010 - DIGITEO - Allan CORNET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

// <-- Non-regression test for bug 6387 -->

//
// <-- Bugzilla URL -->
// http://bugzilla.scilab.org/show_bug.cgi?id=6489
//
// <-- Short Description -->
// removed a wrong dependendy on Wininet.dll 
if getos() == 'Windows' then
  r = dllinfo(SCI+'/bin/windows_tools.dll','imports');
  for i = 1 : size(r)
    if grep(r(i)(1),'/WININET.DLL/i','r') <> [] then pause,end
  end
end


