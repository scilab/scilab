// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2011  - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//=============================================================================
function dlwCompile(files, make_command, makename)

    nf = size(files,"*");

    for i=1:nf
      if ( ilib_verbose() <> 0 ) then
        mprintf(_("   Compilation of ") + string(files(i)) +"\n");
      end
    end

    // then the shared library
    if ( ilib_verbose() <> 0 ) then
      mprintf(_("   Building shared library (be patient)\n"));
    end

    if dlwForceRebuild() then
      target_build = 'clean all';
    else
      target_build = 'all';
    end
    
    [msg, stat] = unix_g(make_command + makename + " " + target_build + " 2>&0");
    if stat <> 0 then
      // more feedback when compilation fails
      [msg, stat, stderr] = unix_g(make_command + makename  + " " + target_build + " 1>&2"); 
      disp(stderr);
      error(msprintf(gettext("%s: Error while executing %s.\n"), "ilib_compile", makename));
    else
      if ilib_verbose() > 1 then
        disp(msg);
      end
    end
endfunction
//=============================================================================
