//==========================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// Copyright (C) DIGITEO - 2009 - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//==========================================
function MSCompiler = findmsvccompiler()



//==========================================
//  functions defined only in findmsvccompiler
//==========================================
function bOK = is_msvc100express()
  try
    MSVS100EXPRESS = winqueryreg('HKEY_LOCAL_MACHINE', ..
                    'Software\Microsoft\VCExpress\10.0\Setup\VS', ..
                    'ProductDir');
    bOK = findWindowsSDK();
  catch
    bOK = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end  
endfunction
//==========================================
function bOK = is_msvc90pro()
  try
    MSVS90Pro = winqueryreg('HKEY_LOCAL_MACHINE', ..
                'Software\Microsoft\VisualStudio\9.0\Setup\VS\Pro', ..
                'ProductDir');
    bOK = %T;
  catch
    bOK = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end  
endfunction
//==========================================
function bOK = is_msvc90std()
  try
    MSVS90std = winqueryreg('HKEY_LOCAL_MACHINE', ..
                'Software\Microsoft\VisualStudio\9.0\Setup\VS\Std', ..
                'ProductDir');
    bOK = %T;
  catch
    bOK = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end  
endfunction
//==========================================
function bOK = is_msvc90express()
  try
    MSVS90EXPRESS = winqueryreg('HKEY_LOCAL_MACHINE', ..
                    'Software\Microsoft\VCExpress\9.0\Setup\VS', ..
                    'ProductDir');
    bOK = findWindowsSDK();
  catch
    bOK = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end  
endfunction
//==========================================
function bOK = is_msvc80pro()
  try
    MSVS80Pro = winqueryreg('HKEY_LOCAL_MACHINE', ..
                'Software\Microsoft\VisualStudio\8.0\Setup\VS\Pro', ..
                'ProductDir');
    bOK = %T;
  catch
    bOK = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end
endfunction
//==========================================
function bOK = is_msvc80std()
  try
    MSVS80std = winqueryreg('HKEY_LOCAL_MACHINE', ..
                'Software\Microsoft\VisualStudio\8.0\Setup\VS\Std', ..
                'ProductDir');
    bOK = %T;
  catch
    bOK = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end  
endfunction
//==========================================
function bOK = is_msvc80express()
  try
    MSVS80EXPRESS = winqueryreg('HKEY_LOCAL_MACHINE', ..
                                'Software\Microsoft\VCExpress\8.0\Setup\VS', ..
                                'ProductDir');
    bOK = findWindowsSDK();
  catch
    bOK = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end  
endfunction
//==========================================
function bOK = is_msvc71()
  try
    MSVS71 = winqueryreg('HKEY_LOCAL_MACHINE', ..
             'SOFTWARE\Microsoft\VisualStudio\7.1\Setup\VC', ..
             'ProductDir');
    bOK = %T;
  catch
    bOK = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end  
endfunction
//==========================================
function bOK = is_msvc70()
  try
    MSVS70 = winqueryreg('HKEY_LOCAL_MACHINE', ..
             'SOFTWARE\Microsoft\VisualStudio\7.0\Setup\VC', ..
             'ProductDir');
    bOK = %T;
  catch
    bOK = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end  
endfunction
//==========================================
function bOK = findWindowsSDK()
  try
    W2003R2SDK = winqueryreg('HKEY_LOCAL_MACHINE', ..
                 'Software\Microsoft\MicrosoftSDK\InstalledSDKs\D2FF9F89-8AA2-4373-8A31-C838BF4DBBE1', ..
                 'Install Dir');
    findSDK1 = %T;
  catch
    findSDK1 = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end

  try
    W2003SDK = winqueryreg('HKEY_LOCAL_MACHINE', ..
               'Software\Microsoft\MicrosoftSDK\InstalledSDKs\8F9E5EF3-A9A5-491B-A889-C58EFFECE8B3', ..
               'Install Dir');
    findSDK2 = %T;
  catch
    findSDK2 = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end
    
  try
    VISTASDK = winqueryreg('HKEY_LOCAL_MACHINE', ..
                           'Software\Microsoft\Microsoft SDKs\Windows', ..
                           'CurrentInstallFolder');
    findSDK3 = %T;
  catch
    findSDK3 = %F;
    // remove last error on 'winqueryreg' fails
    lasterror();
  end
    
  if ( findSDK1 | findSDK2 | findSDK3 ) then 
     bOK = %T;
  else
     TXT = _('Microsoft Visual C++ 2005/2008 Express Edition has been detected,') + char(10) + ..
           _('but not Microsoft Platform SDK for Windows Server 2003 R2 or more.') + char(10) + ..
           _('Please install this SDK if you want to use dynamic link with scilab.');
     warning(TXT);
     bOK = %F;
  end

endfunction
//==========================================


  MSCompiler='unknown'; // unknown
  
  if MSDOS then

    if is_msvc100express() then
      MSCompiler = 'msvc100express';          // Microsoft Visual 2010 Express
      return;
    end
  
    if is_msvc90pro() then
      MSCompiler = 'msvc90pro';          // Microsoft Visual 2008 Studio Professional
      return;
    end
      
    if is_msvc90std() then
      MSCompiler = 'msvc90std';          // Microsoft Visual 2008 Studio Standard      
      return;
    end
      
    if is_msvc90express() then
      MSCompiler = 'msvc90express';      // Microsoft Visual 2008 Express 
      return;
    end
      
    if is_msvc80pro() then
      MSCompiler = 'msvc80pro';          // Microsoft Visual 2005 Studio Professional
      return;
    end

    if is_msvc80std() then
      MSCompiler = 'msvc80std';          // Microsoft Visual 2005 Studio Standard
      return;
    end
      
    if is_msvc80express() then
      MSCompiler = 'msvc80express';      // Microsoft Visual 2005 Express
      return;
    end
      
    if is_msvc71() then
      MSCompiler = 'msvc71';             // Microsoft Visual Studio .NET 2003
      return;
    end
      
    if is_msvc70() then
      MSCompiler = 'msvc70';             // Microsoft Visual Studio .NET 2002
      return;
    end
    
  else // NOT MSDOS
    MSCompiler = 'unknown'; // unknown
  end
endfunction
//==========================================

  