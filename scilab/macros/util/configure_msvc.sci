//-----------------------------------------------------------------------------
// Allan CORNET
// INRIA 2005
//-----------------------------------------------------------------------------
function bOK=configure_msvc()
if MSDOS then
  msvc=findmsvccompiler();
  select msvc,
  case 'msvc80pro' then
      bOK=setmsvc80pro()
  case 'msvc80std' then
      bOK=setmsvc80std()    
  case 'msvc80express' then
      bOK=setmsvc80express()
  case 'msvc71' then
      bOK=setmsvc71()
  case 'msvc70' then
      bOK=setmsvc70()
  case 'msvc60' then
      bOK=setmsvc60()
  case 'msvc50' then
      bOK=setmsvc50()
  else
      //disp('Warning Ms Visual C Compiler not found.');
      bOK=%F;
  end
else
  bOK=%F;
end
endfunction
//-----------------------------------------------------------------------------
function bOK=setmsvc80pro()
if MSDOS then
  MSVSDir=winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\VisualStudio\8.0\Setup\VS\Pro','ProductDir');
  if ( part(MSVSDir,length(MSVSDir)) == '\' ) then MSVSDir=part(MSVSDir,1:length(MSVSDir)-1);end;
  
  err=setenv('VSINSTALLDIR',MSVSDir);
  if (err == %F) then bOK=%F,return,end
  
  MSVCDir=MSVSDir+'\VC';
  err=setenv('VCINSTALLDIR',MSVCDir);
  if (err == %F) then bOK=%F,return,end
  
  DevEnvDir=MSVSDir+'\Common7\IDE';
  err=setenv('DevEnvDir',DevEnvDir);
  if (err == %F) then bOK=%F,return,end
  
  PATH=getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK=%F,return,end
  
  err=setenv('PATH',DevEnvDir+';'+MSVCDir+'\bin;'+MSVSDir+'\Common7\Tools;'+MSVSDir+'\SDK\v2.0\bin;'+MSVCDir+'\VCPackages;'+PATH+";"+WSCI+"\bin;");
  if (err == %F) then bOK=%F,return,end
  
  INCLUDE=getenv('INCLUDE','');  
  INCLUDE=MSVCDir+'\INCLUDE;'+MSVCDir+'\PlatformSDK\include;'+MSVSDir+'\SDK\v2.0\include';
    
  err=setenv("INCLUDE",INCLUDE);
  if (err == %F) then bOK=%F,return,end
  
  LIB=getenv('LIB','');  
  LIB=MSVCDir+'\LIB;'+MSVSDir+'\SDK\v2.0\lib;'+MSVSDir+'\VC\PlatformSDK\lib;'+LIB;
  err=setenv("LIB",LIB);
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("USE_MT","-MT");
  if (err == %F) then bOK=%F,return,end
    
  bOK=%T
else
  bOK=%F;
end
endfunction
//-----------------------------------------------------------------------------
function bOK=setmsvc80std()
if MSDOS then
  MSVSDir=winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\VisualStudio\8.0\Setup\VS\Std','ProductDir');
  if ( part(MSVSDir,length(MSVSDir)) == '\' ) then MSVSDir=part(MSVSDir,1:length(MSVSDir)-1);end;
  
  err=setenv('VSINSTALLDIR',MSVSDir);
  if (err == %F) then bOK=%F,return,end
  
  MSVCDir=MSVSDir+'\VC';
  err=setenv('VCINSTALLDIR',MSVCDir);
  if (err == %F) then bOK=%F,return,end
  
  DevEnvDir=MSVSDir+'\Common7\IDE';
  err=setenv('DevEnvDir',DevEnvDir);
  if (err == %F) then bOK=%F,return,end
  
  PATH=getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK=%F,return,end
  
  err=setenv('PATH',DevEnvDir+';'+MSVCDir+'\bin;'+MSVSDir+'\Common7\Tools;'+MSVSDir+'\SDK\v2.0\bin;'+MSVCDir+'\VCPackages;'+PATH+";"+WSCI+"\bin;");
  if (err == %F) then bOK=%F,return,end
  
  INCLUDE=getenv('INCLUDE','');  
  INCLUDE=MSVCDir+'\INCLUDE;'+MSVCDir+'\PlatformSDK\include;'+MSVSDir+'\SDK\v2.0\include';
  err=setenv("INCLUDE",INCLUDE);
  if (err == %F) then bOK=%F,return,end
  
  LIB=getenv('LIB','');  
  LIB=MSVCDir+'\LIB;'+MSVSDir+'\SDK\v2.0\lib;'+MSVSDir+'\VC\PlatformSDK\lib;'+LIB;
  err=setenv("LIB",LIB);
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("USE_MT","-MT");
  if (err == %F) then bOK=%F,return,end
  
  bOK=%T
else
  bOK=%F;
end
endfunction
//-----------------------------------------------------------------------------
function bOK=setmsvc80express()
if MSDOS then
  MSVSDir=winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\VCExpress\8.0\Setup\VS','ProductDir');
  if ( part(MSVSDir,length(MSVSDir)) == '\' ) then MSVSDir=part(MSVSDir,1:length(MSVSDir)-1);end;
  
  err=setenv('VSINSTALLDIR',MSVSDir);
  if (err == %F) then bOK=%F,return,end
  
  MSVCDir=MSVSDir+'\VC';
  err=setenv('VCINSTALLDIR',MSVCDir);
  if (err == %F) then bOK=%F,return,end
  
  DevEnvDir=MSVSDir+'\Common7\IDE';
  err=setenv('DevEnvDir',DevEnvDir);
  if (err == %F) then bOK=%F,return,end
  
  PATH=getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK=%F,return,end
  
  err=setenv('PATH',DevEnvDir+';'+MSVCDir+'\bin;'+MSVSDir+'\Common7\Tools;'+MSVSDir+'\SDK\v2.0\bin;'+MSVCDir+'\VCPackages;'+PATH+";"+WSCI+"\bin;");
  if (err == %F) then bOK=%F,return,end
  
  ierr1=execstr("VISTASDK=winqueryreg(''HKEY_LOCAL_MACHINE'',''Software\Microsoft\Microsoft SDKs\Windows'',''CurrentInstallFolder'');","errcatch");  
  ierr2=execstr("W2003R2SDK=winqueryreg(''HKEY_LOCAL_MACHINE'',''Software\Microsoft\MicrosoftSDK\InstalledSDKs\D2FF9F89-8AA2-4373-8A31-C838BF4DBBE1'',''Install Dir'');","errcatch");
  ierr3=execstr("W2003SDK=winqueryreg(''HKEY_LOCAL_MACHINE'',''Software\Microsoft\MicrosoftSDK\InstalledSDKs\8F9E5EF3-A9A5-491B-A889-C58EFFECE8B3'',''Install Dir'');","errcatch");
  
  if (ierr1 == 0) then
    WINDOWSSDK = winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\Microsoft SDKs\Windows','CurrentInstallFolder');
    lasterror(%T); // The error message is cleared
  else 
    if (ierr2 == 0) then
      WINDOWSSDK = winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\MicrosoftSDK\InstalledSDKs\D2FF9F89-8AA2-4373-8A31-C838BF4DBBE1','Install Dir');
      lasterror(%T); // The error message is cleared
    else
      if (ierr3 == 0) then
        WINDOWSSDK = winqueryreg('HKEY_LOCAL_MACHINE','Software\Microsoft\MicrosoftSDK\InstalledSDKs\8F9E5EF3-A9A5-491B-A889-C58EFFECE8B3','Install Dir');
        lasterror(%T); // The error message is cleared
      end
    end
  end
  
  INCLUDE=getenv('INCLUDE','');  
  INCLUDE=MSVCDir+'\INCLUDE;'+WINDOWSSDK+'INCLUDE;'
  err=setenv("INCLUDE",INCLUDE);
  if (err == %F) then bOK=%F,return,end
  
  LIB=getenv('LIB',''); 
  LIB=MSVCDir+'\LIB;'+MSVSDir+'\SDK\v2.0\lib;'+WINDOWSSDK+'Lib;'+LIB;
  err=setenv("LIB",LIB);
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("USE_MT","-MT");
  if (err == %F) then bOK=%F,return,end
  
  bOK=%T
else
  bOK=%F;
end
endfunction
//-----------------------------------------------------------------------------
function bOK=setmsvc71()
if MSDOS then
  MSVCDir=winqueryreg('HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\VisualStudio\7.1\Setup\VC','ProductDir')
  if ( part(MSVCDir,length(MSVCDir)) == '\' ) then MSVCDir=part(MSVCDir,1:length(MSVCDir)-1),end;

  err=setenv('MSVCDir',MSVCDir);
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("DevEnvDir",MSVCDir+"\..\Common7\Tools");
  if (err == %F) then bOK=%F,return,end
  
  PATH=getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK=%F,return,end

  DevEnvDir=getenv('DevEnvDir','ndef');
  if (DevEnvDir =='ndef') then bOK=%F,return,end
  
  err=setenv("PATH",MSVCDir+"\BIN;"+DevEnvDir+";"+DevEnvDir+"\bin;"+MSVCDir+"\..\Common7\IDE;"+PATH+";"+WSCI+"\bin;");
  if (err == %F) then bOK=%F,return,end
  
  INCLUDE=getenv('INCLUDE','');
    
  err=setenv("INCLUDE",MSVCDir+"\ATLMFC\INCLUDE;"+MSVCDir+"\INCLUDE;"+MSVCDir+"\PlatformSDK\include;"+INCLUDE);
  if (err == %F) then bOK=%F,return,end
  
  LIB=getenv('LIB','ndef');
    
  err=setenv("LIB",MSVCDir+"\ATLMFC\LIB;"+MSVCDir+"\LIB;"+MSVCDir+"\PlatformSDK\lib;"+LIB);
  if (err == %F) then bOK=%F,return,end

  bOK=%T
else
  bOK=%F;
end
endfunction
//-----------------------------------------------------------------------------
function bOK=setmsvc70()
if MSDOS then
  MSVCDir=winqueryreg('HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\VisualStudio\7.0\Setup\VC','ProductDir');
  if ( part(MSVCDir,length(MSVCDir)) == '\' ) then MSVCDir=part(MSVCDir,1:length(MSVCDir)-1),end;
  
  err=setenv('MSVCDir',MSVCDir);
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("DevEnvDir",MSVCDir+"\..\Common7\Tools");
  if (err == %F) then bOK=%F,return,end
  
  DevEnvDir=getenv('DevEnvDir','ndef');
  if (DevEnvDir =='ndef') then bOK=%F,return,end
  
  PATH=getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK=%F,return,end
  
  err=setenv("PATH",MSVCDir+"\BIN;"+DevEnvDir+";"+DevEnvDir+"\bin;"+MSVCDir+"\..\Common7\IDE;"+PATH+";"+SCI+"\bin;");
  if (err == %F) then bOK=%F,return,end
  
  INCLUDE=getenv('INCLUDE','');
    
  err=setenv("INCLUDE",MSVCDir+"\ATLMFC\INCLUDE;"+MSVCDir+"\INCLUDE;"+MSVCDir+"\PlatformSDK\include;"+INCLUDE);
  if (err == %F) then bOK=%F,return,end
  
  LIB=getenv('LIB','');
    
  err=setenv("LIB",MSVCDir+"\ATLMFC\LIB;"+MSVCDir+"\LIB;"+MSVCDir+"\PlatformSDK\lib;"+LIB);
  if (err == %F) then bOK=%F,return,end
  
  bOK=%T
else
  bOK=%F;
end
endfunction
//-----------------------------------------------------------------------------
function bOK=setmsvc60()
if MSDOS then
  MSVCDir=winqueryreg('HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\DevStudio\6.0\Products\Microsoft Visual C++','ProductDir');
  if ( part(MSVCDir,length(MSVCDir)) == '\' ) then MSVCDir=part(MSVCDir,1:length(MSVCDir)-1),end;
  
  err=setenv('MSVCDir',MSVCDir);
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("MSDevDir",MSVCDir+"\..\Common\msdev98");
  if (err == %F) then bOK=%F,return,end
  
  MSDevDir=getenv('MSDevDir','ndef');
  if (MSDevDir =='ndef') then bOK=%F,return,end
  
  PATH=getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK=%F,return,end

  err=setenv("PATH",MSVCDir+'\BIN;'+MSDevDir+'\bin;'+';'+WSCI+'\bin;'+PATH);
  if (err == %F) then bOK=%F,return,end
  
  INCLUDE=getenv('INCLUDE','');
  
	err=setenv("INCLUDE",MSVCDir+'\INCLUDE;'+MSVCDir+'\MFC\INCLUDE;'+MSVCDir+'\ATL\INCLUDE;'+INCLUDE);
	if (err == %F) then bOK=%F,return,end
	
	LIB=getenv('LIB','');
    
  err=setenv("LIB",MSVCDir+'\LIB;'+MSVCDir+'\MFC\LIB;'+LIB);
  if (err == %F) then bOK=%F,return,end
  
  bOK=%T
else
  bOK=%F;
end
endfunction
//-----------------------------------------------------------------------------
function bOK=setmsvc50()
if MSDOS then
  MSVCDir=winqueryreg('HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\DevStudio\5.0\Directories','ProductDir');
  if ( part(MSVCDir,length(MSVCDir)) == '\' ) then MSVCDir=part(MSVCDir,1:length(MSVCDir)-1),end;
  
  err=setenv('MSVCDir',MSVCDir);
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("MSDevDir",MSVCDir+"\..\sharedIDE");
  if (err == %F) then bOK=%F,return,end
  
  MSDevDir=getenv('MSDevDir','ndef');
  if (MSDevDir =='ndef') then bOK=%F,return,end
  
  PATH=getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK=%F,return,end
  
  err=setenv("PATH",MSVCDir+'\BIN;'+MSDevDir+'\bin;'+';'+WSCI+'\bin;'+PATH);
  if (err == %F) then bOK=%F,return,end
  
  INCLUDE=getenv('INCLUDE','');
  
	err=setenv("INCLUDE",MSVCDir+'\INCLUDE;'+MSVCDir+'\MFC\INCLUDE;'+MSVCDir+'\ATL\INCLUDE;'+INCLUDE);
	if (err == %F) then bOK=%F,return,end
	
	LIB=getenv('LIB','');
    
  err=setenv("LIB",MSVCDir+'\LIB;'+MSVCDir+'\MFC\LIB;'+LIB);
  if (err == %F) then bOK=%F,return,end
  
  bOK=%T
else
  bOK=%F;
end
endfunction
//-----------------------------------------------------------------------------
