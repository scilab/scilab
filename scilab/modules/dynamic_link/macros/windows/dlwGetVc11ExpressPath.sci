// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) Scilab Enterprises - 2012 - Antoine ELIAS
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//=============================================================================
function vcPath = dlwGetVc11ExpressPath()
  vcPath = [];
  try
    vcPath = winqueryreg('HKEY_LOCAL_MACHINE', ..
                      'Software\Microsoft\VCExpress\11.0\Setup\VC', ..
                      'ProductDir');
  catch
    // remove last error on 'winqueryreg' fails
    lasterror();
    return;
  end

  //remove VC in path
  if isdir(vcPath) then
      temp=pwd();cd(vcPath);cd("..");vcPath = pwd() + "\";cd(temp);   
      // remove last file separator if it exists
      if vcPath <> [] then
        vcPath = pathconvert(vcPath, %f, %t);
      end
  else
    vcPath = [];
  end      
endfunction
//=============================================================================
