// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2012 - Antoine ELIAS
//
// Copyright (C) 2012 - 2016 - Scilab Enterprises
//
// This file is hereby licensed under the terms of the GNU GPL v2.0,
// pursuant to article 5.3.4 of the CeCILL v.2.1.
// This file was originally licensed under the terms of the CeCILL v2.1,
// and continues to be available under such terms.
// For more information, see the COPYING file which you should have received
// along with this program.

//=============================================================================
function vcPath = dlwGetVc11ProPath()
  vcPath = [];
  try
    vcPath = winqueryreg('HKEY_LOCAL_MACHINE', ..
                     'Software\Microsoft\VisualStudio\11.0\Setup\VS', ..
                     'ProductDir');
  catch
    // remove last error on 'winqueryreg' fails
    lasterror();
    return;
  end

  // remove last file separator if it exists
  if vcPath <> [] then
    vcPath = pathconvert(vcPath, %f, %t);
  end

endfunction
//=============================================================================
