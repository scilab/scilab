//==========================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt

//==========================================
function bOK = configure_msvc()


//==========================================
//  functions defined only in configure_msvc
//==========================================
function bOK = set_msvc90pro()
  // NEED TO INSTALL .NET 2008 Pro to check
  bOK = %F;
  try
    MSVSDir = winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\VisualStudio\9.0\Setup\VS\Pro','ProductDir');
  catch
    MSVSDir = '';
  end
  bOK = commons_msvc90(MSVSDir);
endfunction
//==========================================
function bOK = set_msvc90std()
  // NEED TO INSTALL .NET 2008 Std to check
  bOK = %F;
  try
    MSVSDir = winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\VisualStudio\9.0\Setup\VS\Std','ProductDir');
  catch
    MSVSDir = '';
  end
  bOK = commons_msvc90(MSVSDir);
endfunction
//==========================================
function bOK = set_msvc90express()
  bOK = %F;
  try
    MSVSDir = winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\VCExpress\9.0\Setup\VS','ProductDir');
  catch
    return;
  end
  if ( part(MSVSDir,length(MSVSDir)) == filesep() ) then MSVSDir = part(MSVSDir,1:length(MSVSDir)-1);end;
  
  err = setenv('VSINSTALLDIR',MSVSDir);
  if (err == %F) then bOK = %F,return,end
  
  MSVCDir = MSVSDir + filesep() + 'VC';
  err = setenv('VCINSTALLDIR',MSVCDir);
  if (err == %F) then bOK = %F,return,end
  
  DevEnvDir = MSVSDir + filesep() +'Common7\IDE';
  err = setenv('DevEnvDir',DevEnvDir);
  if (err == %F) then bOK = %F,return,end
  
  PATH = getenv('PATH','ndef');
  if (PATH == 'ndef') then  bOK = %F,return,end
  
  err = setenv('PATH',DevEnvDir + ';' + ..
               MSVCDir + filesep() + 'bin' + pathsep() + ..
               MSVSDir + filesep() + 'Common7\Tools' + pathsep() + ..
               MSVSDir + filesep() + 'SDK\v2.0\bin' + pathsep() + .. 
               MSVCDir + filesep() + 'VCPackages' + pathsep() + ..
               PATH + pathsep() + ..
               WSCI + filesep() + 'bin' + pathsep() );
  if (err == %F) then bOK = %F,return,end
  
  WINDOWSSDK = get_ms_SDK();
  
  if WINDOWSSDK <> '' then
    INCLUDE = getenv('INCLUDE','');  
    INCLUDE = MSVCDir + filesep() + 'INCLUDE' + pathsep() + ..
              WINDOWSSDK + 'INCLUDE' + pathsep();
    err = setenv('INCLUDE',INCLUDE);
    if (err == %F) then bOK = %F,return,end
  
    LIB = getenv('LIB',''); 
    LIB = MSVCDir + filesep() + 'LIB' + pathsep() + ..
          MSVSDir + filesep() + 'SDK\v2.0\lib' + pathsep() + ..
          WINDOWSSDK + 'Lib' + pathsep() + ..
          LIB;
    err = setenv('LIB',LIB);
    if (err == %F) then bOK = %F,return,end

    bOK = %T;
  end
endfunction
//==========================================
function bOK = set_msvc80pro()
  bOK = %F;
  try
    MSVSDir = winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\VisualStudio\8.0\Setup\VS\Pro','ProductDir');
  catch
    MSVSDir = '';
  end
  bOK = commons_msvc80(MSVSDir);
endfunction
//==========================================
function bOK = set_msvc80std()
  bOK = %F;
  try
    MSVSDir = winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\VisualStudio\8.0\Setup\VS\Std','ProductDir');
  catch
    MSVSDir = '';
  end
  bOK = commons_msvc80(MSVSDir);
endfunction
//==========================================
function bOK = set_msvc80express()
  bOK = %F;
  try
    MSVSDir = winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\VCExpress\8.0\Setup\VS','ProductDir');
  catch
    return;
  end
  if ( part(MSVSDir,length(MSVSDir)) == filesep() ) then MSVSDir = part(MSVSDir,1:length(MSVSDir)-1);end;
  
  err = setenv('VSINSTALLDIR',MSVSDir);
  if (err == %F) then bOK = %F,return,end
  
  MSVCDir = MSVSDir + filesep() + 'VC';
  err = setenv('VCINSTALLDIR',MSVCDir);
  if (err == %F) then bOK = %F,return,end
  
  DevEnvDir = MSVSDir + filesep() +'Common7\IDE';
  err = setenv('DevEnvDir',DevEnvDir);
  if (err == %F) then bOK = %F,return,end
  
  PATH = getenv('PATH','ndef');
  if (PATH == 'ndef') then  bOK = %F,return,end
  
  err = setenv('PATH',DevEnvDir + pathsep() + ..
               MSVCDir + filesep() + 'bin' + pathsep() + ..
               MSVSDir + filesep() + 'Common7\Tools' + pathsep() + ..
               MSVSDir + filesep() + 'SDK\v2.0\bin' + pathsep() + .. 
               MSVCDir + filesep() + 'VCPackages' + pathsep() + ..
               PATH + pathsep() + ..
               WSCI + filesep() + 'bin' + pathsep() );
  if (err == %F) then bOK=%F,return,end
  
  WINDOWSSDK = get_ms_SDK();
  
  if WINDOWSSDK <> '' then
    INCLUDE = getenv('INCLUDE','');  
    INCLUDE = MSVCDir + filesep() + 'INCLUDE' + pathsep() + ..
              WINDOWSSDK + 'INCLUDE' + pathsep();
    err = setenv('INCLUDE',INCLUDE);
    if (err == %F) then bOK = %F,return,end
  
    LIB = getenv('LIB',''); 
    LIB = MSVCDir + filesep() + 'LIB' + pathsep() + ..
          MSVSDir + filesep() + 'SDK\v2.0\lib' + pathsep() + ..
          WINDOWSSDK + 'Lib' + pathsep() + ..
          LIB;
    err = setenv('LIB',LIB);
    if (err == %F) then bOK = %F,return,end

    bOK = %T;
  end

endfunction
//==========================================
function bOK = set_msvc71()
  bOK = %F;
  try 
    MSVCDir = winqueryreg('HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\VisualStudio\7.1\Setup\VC','ProductDir');
  catch
    return;
  end
  if ( part(MSVCDir,length(MSVCDir)) == filesep() ) then MSVCDir = part(MSVCDir,1:length(MSVCDir)-1),end;

  err = setenv('MSVCDir',MSVCDir);
  if (err == %F) then bOK = %F,return,end
  
  err=setenv('DevEnvDir',MSVCDir + filesep() + '..\Common7\Tools');
  if (err == %F) then bOK = %F,return,end
  
  PATH = getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK = %F,return,end

  DevEnvDir = getenv('DevEnvDir','ndef');
  if (DevEnvDir =='ndef') then bOK = %F,return,end
  
  err = setenv('PATH',MSVCDir + filesep() + 'BIN' + pathsep() + ..
               DevEnvDir + pathsep() + ..
               DevEnvDir + filesep() + 'bin' + pathsep() + ..
               MSVCDir + filesep() + '..\Common7\IDE' + pathsep() + ..
               PATH + pathsep() + ..
               WSCI + filesep() + 'bin' + pathsep());
  if (err == %F) then bOK = %F,return,end
  
  INCLUDE = getenv('INCLUDE','');
    
  err = setenv('INCLUDE',MSVCDir + filesep() + 'ATLMFC\INCLUDE' + pathsep() + ..
               MSVCDir + filesep() + 'INCLUDE' + pathsep() + ..
               MSVCDir + filesep() + 'PlatformSDK\include' + pathsep() + ..
               INCLUDE);
  if (err == %F) then bOK = %F,return,end
  
  LIB = getenv('LIB','ndef');
    
  err = setenv('LIB',MSVCDir + filesep() + 'ATLMFC\LIB' + pathsep() + ..
               MSVCDir + filesep() + 'LIB' + pathsep() + ..
               MSVCDir + filesep() + 'PlatformSDK\lib' + pathsep() + ..
               LIB);
  if (err == %F) then bOK = %F,return,end

  bOK=%T;
endfunction
//==========================================
function bOK = set_msvc70()
  bOK = %F;
  try
    MSVCDir = winqueryreg('HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\VisualStudio\7.0\Setup\VC','ProductDir');
  catch
    return;
  end
  if ( part(MSVCDir,length(MSVCDir)) == filesep() ) then MSVCDir = part(MSVCDir,1:length(MSVCDir)-1),end;
  
  err = setenv('MSVCDir',MSVCDir);
  if (err == %F) then bOK = %F,return,end
  
  err = setenv('DevEnvDir',MSVCDir + filesep() + '..\Common7\Tools');
  if (err == %F) then bOK = %F,return,end
  
  DevEnvDir = getenv('DevEnvDir','ndef');
  if (DevEnvDir =='ndef') then bOK = %F,return,end
  
  PATH=getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK = %F,return,end
  
  err = setenv('PATH',MSVCDir + filesep() + 'BIN;' + ..
               DevEnvDir + pathsep() + ..
               DevEnvDir + filesep() + 'bin' + pathsep() + ..
               MSVCDir + filesep() + '..\Common7\IDE' + pathsep() + ..
               PATH + pathsep() + ..
               SCI + filesep() + 'bin' + pathsep() );
  if (err == %F) then bOK=%F,return,end
  
  INCLUDE = getenv('INCLUDE','');
    
  err = setenv('INCLUDE',MSVCDir + filesep() + 'ATLMFC\INCLUDE' + pathsep() + ..
               MSVCDir + filesep() + 'INCLUDE' + pathsep() +..
               MSVCDir + filesep() + 'PlatformSDK\include' + pathsep() + ..
               INCLUDE);
  if (err == %F) then bOK=%F,return,end
  
  LIB = getenv('LIB','');
    
  err = setenv('LIB',MSVCDir + filesep() + 'ATLMFC\LIB' + pathsep() + ..
               MSVCDir + filesep() + 'LIB'+ pathsep() +..
               MSVCDir + filesep() + 'PlatformSDK\lib' + pathsep() + ..
               LIB);
  if (err == %F) then bOK = %F,return,end
  
  bOK = %T;

endfunction
//==========================================
function SDKpath = get_ms_SDK()
  // We always use last version of MS SDK
  try
    // Vista SDK
    SDKpath = winqueryreg('HKEY_LOCAL_MACHINE', ..
                          'Software\Microsoft\Microsoft SDKs\Windows', ..
                          'CurrentInstallFolder');
    return;
  catch
    try
      // Windows 2003 R2 SDK
      SDKpath = winqueryreg('HKEY_LOCAL_MACHINE', ..
                            'Software\Microsoft\MicrosoftSDK\InstalledSDKs\D2FF9F89-8AA2-4373-8A31-C838BF4DBBE1', ..
                            'Install Dir');
      return;
    catch
      try
        // Windows 2003 SDK
        SDKpath = winqueryreg('HKEY_LOCAL_MACHINE', ..
                              'Software\Microsoft\MicrosoftSDK\InstalledSDKs\8F9E5EF3-A9A5-491B-A889-C58EFFECE8B3', ..
                              'Install Dir');
        return;
      catch
        SDKpath = '';
      end
    end
  end
endfunction
//==========================================
function bOK = commons_msvc80(MS_VS_DIRECTORY)
  bOK = %F;
  if (MS_VS_DIRECTORY <> '') then
    if ( part(MS_VS_DIRECTORY,length(MS_VS_DIRECTORY)) == filesep() ) then 
      MSVSDir = part(MS_VS_DIRECTORY,1:length(MS_VS_DIRECTORY)-1);
    end;
  
    err = setenv('VSINSTALLDIR',MSVSDir);
    if (err == %F) then bOK = %F,return,end
  
    MSVCDir = MSVSDir + filesep() + 'VC';
    err = setenv('VCINSTALLDIR',MSVCDir);
    if (err == %F) then bOK = %F,return,end
  
    DevEnvDir = MSVSDir + filesep() + 'Common7\IDE';
    err = setenv('DevEnvDir',DevEnvDir);
    if (err == %F) then bOK = %F,return,end
  
    PATH = getenv('PATH','ndef');
    if (PATH == 'ndef') then  bOK = %F,return,end
  
    err=setenv('PATH',DevEnvDir + pathsep() + ..
               MSVCDir + filesep() + 'bin' + pathsep() + ..
               MSVSDir + filesep() + 'Common7\Tools'+ pathsep() + ..
               MSVSDir + filesep() + 'SDK\v2.0\bin' + pathsep() + ..
               MSVCDir + filesep() + 'VCPackages' + pathsep() + ..
               PATH + pathsep() + ..
               WSCI + filesep() + 'bin' + pathsep());
    if (err == %F) then bOK=%F,return,end
  
    INCLUDE = getenv('INCLUDE','');  
    INCLUDE = MSVCDir + filesep() + 'INCLUDE' + pathsep() + ..
              MSVCDir + filesep() + 'PlatformSDK\include' + pathsep() + ..
              MSVSDir + filesep() + 'SDK\v2.0\include' + pathsep() + ..
              INCLUDE;
    
    err = setenv('INCLUDE',INCLUDE);
    if (err == %F) then bOK = %F,return,end
  
    LIB = getenv('LIB','');  
    LIB = MSVCDir + filesep() + 'LIB' + pathsep() + ..
          MSVSDir + filesep() + 'SDK\v2.0\lib' + pathsep() + ..
          MSVSDir + filesep() + 'VC\PlatformSDK\lib' + pathsep() + ..
          LIB;
    err = setenv('LIB',LIB);
    if (err == %F) then bOK = %F,return,end
  
    bOK = %T;
  end
endfunction
//==========================================
function bOK = commons_msvc90(MS_VS_DIRECTORY)
  bOK = %F;
  if (MS_VS_DIRECTORY <> '') then
  end
endfunction
//==========================================

  if MSDOS then
    msvc = findmsvccompiler();
    bOK = %F;
    
    select msvc,
      case  'msvc90pro' then          // Microsoft Visual 2008 Studio Professional
        bOK = set_msvc90pro();
        
      case  'msvc90std' then          // Microsoft Visual 2008 Studio Standard      
        bOK = set_msvc90std();
        
      case  'msvc90express' then      // Microsoft Visual 2008 Express 
        bOK = set_msvc90express();
        
      case  'msvc80pro' then          // Microsoft Visual 2005 Studio Professional
        bOK = set_msvc80pro();
        
      case  'msvc80std' then          // Microsoft Visual 2005 Studio Standard
        bOK = set_msvc80std();    
        
      case  'msvc80express' then      // Microsoft Visual 2005 Express
        bOK = set_msvc80express();    
        
      case  'msvc71' then             // Microsoft Visual Studio .NET 2003
        bOK = set_msvc71();    
        
      case  'msvc70' then             // Microsoft Visual Studio .NET 2002
        bOK = set_msvc70();  
      
      else
    		warning('Microsoft Visual Studio C Compiler not found.');
        bOK = %F;
      end
  else
    bOK = %F;
  end
  
endfunction
//==========================================


