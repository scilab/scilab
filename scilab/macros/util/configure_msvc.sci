//-----------------------------------------------------------------------------
// Allan CORNET
// INRIA 2005
//-----------------------------------------------------------------------------
function bOK=configure_msvc()
if MSDOS then
  msvc=findmsvccompiler();
  select msvc,
  case 'msvc71' then
      bOK=setmsvc71()
  case 'msvc70' then
      bOK=setmsvc70()
  case 'msvc6' then
      bOK=setmsvc6()
  case 'msvc5' then
      bOK=setmsvc5()
  else
      disp('Warning Ms Visual C Compiler not found.');
      bOK=%F;
  end
else
  bOK=%F;
end
endfunction
//-----------------------------------------------------------------------------
function bOK=setmsvc71()
if MSDOS then
  MSVCDir=winqueryreg('HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\VisualStudio\7.1\Setup\VC','ProductDir')
  MSVCDir=part(MSVCDir,1:length(MSVCDir)-1)

  err=setenv('MSVCDir',MSVCDir);
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("DevEnvDir",MSVCDir+"\..\Common7\Tools");
  if (err == %F) then bOK=%F,return,end
  
  PATH=getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK=%F,return,end
  
  DevEnvDir=getenv('DevEnvDir','ndef');
  if (DevEnvDir =='ndef') then bOK=%F,return,end
  
  err=setenv("PATH",MSVCDir+"\BIN;"+DevEnvDir+";"+DevEnvDir+"\bin;"+MSVCDir+"\..\Common7\IDE;"+PATH+";"+SCI+";");
  if (err == %F) then bOK=%F,return,end
  
  INCLUDE=getenv('INCLUDE','ndef');
  if (INCLUDE =='ndef') then  bOK=%F,return,end
  
  err=setenv("INCLUDE",MSVCDir+"\ATLMFC\INCLUDE;"+MSVCDir+"\INCLUDE;"+MSVCDir+"\PlatformSDK\include;"+INCLUDE);
  if (err == %F) then bOK=%F,return,end
  
  LIB=getenv('LIB','ndef');
  if (LIB =='ndef') then  bOK=%F,return,end
  
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
  MSVCDir=part(MSVCDir,1:length(MSVCDir)-1)
  
  err=setenv('MSVCDir',MSVCDir);
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("DevEnvDir",MSVCDir+"\..\Common7\Tools");
  if (err == %F) then bOK=%F,return,end
  
  DevEnvDir=getenv('DevEnvDir','ndef');
  if (DevEnvDir =='ndef') then bOK=%F,return,end
  
  PATH=getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK=%F,return,end
  
  err=setenv("PATH",MSVCDir+"\BIN;"+DevEnvDir+";"+DevEnvDir+"\bin;"+MSVCDir+"\..\Common7\IDE;"+PATH+";"+SCI+";");
  if (err == %F) then bOK=%F,return,end
  
  INCLUDE=getenv('INCLUDE','ndef');
  if (INCLUDE =='ndef') then  bOK=%F,return,end
  
  err=setenv("INCLUDE",MSVCDir+"\ATLMFC\INCLUDE;"+MSVCDir+"\INCLUDE;"+MSVCDir+"\PlatformSDK\include;"+INCLUDE);
  if (err == %F) then bOK=%F,return,end
  
  LIB=getenv('LIB','ndef');
  if (LIB =='ndef') then  bOK=%F,return,end
  
  err=setenv("LIB",MSVCDir+"\ATLMFC\LIB;"+MSVCDir+"\LIB;"+MSVCDir+"\PlatformSDK\lib;"+LIB);
  if (err == %F) then bOK=%F,return,end
  
  bOK=%T
else
  bOK=%F;
end
endfuntion
//-----------------------------------------------------------------------------
function bOK=setmsvc60()

if MSDOS then
  MSVCDir=winqueryreg('HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\DevStudio\6.0\Products\Microsoft Visual C++','ProductDir');
  MSVCDir=part(MSVCDir,1:length(MSVCDir)-1)
  
  err=setenv('MSVCDir',MSVCDir);
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("MSDevDir",MSVCDir+"\..\Common\msdev98");
  if (err == %F) then bOK=%F,return,end
  
  PATH=getenv('PATH','ndef');
  if (PATH =='ndef') then  bOK=%F,return,end

  err=setenv("PATH",MSVCDir+'\BIN;'+MSDevDir+'\bin;'+PATH);
  if (err == %F) then bOK=%F,return,end
  
  INCLUDE=getenv('INCLUDE','ndef');
  if (INCLUDE =='ndef') then  bOK=%F,return,end

	err=setenv("INCLUDE",MSVCDir+'\INCLUDE;'+MSVCDir+'\MFC\INCLUDE;'+MSVCDir+'\ATL\INCLUDE;'+INCLUDE);
	if (err == %F) then bOK=%F,return,end
	
	LIB=getenv('LIB','ndef');
  if (LIB =='ndef') then  bOK=%F,return,end
  
  err=setenv("LIB",MSVCDir+'\LIB;'+MSVCDir+'\MFC\LIB;'+LIB);
  if (err == %F) then bOK=%F,return,end
  
  bOK=%T
else
  bOK=%F;
end
endfuntion
//-----------------------------------------------------------------------------
function bOK=setmsvc50()
if MSDOS then
  MSVCDir=winqueryreg('HKEY_LOCAL_MACHINE','SOFTWARE\Microsoft\DevStudio\5.0\Directories','ProductDir');
  MSVCDir=part(MSVCDir,1:length(MSVCDir)-1)
  
  err=setenv('MSVCDir',MSVCDir);
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("MSDevDir",MSVCDir+"\..\sharedIDE");
  if (err == %F) then bOK=%F,return,end
  
  err=setenv("PATH",MSVCDir+'\BIN;'+MSDevDir+'\bin;'+PATH);
  if (err == %F) then bOK=%F,return,end
  
  INCLUDE=getenv('INCLUDE','ndef');
  if (INCLUDE =='ndef') then  bOK=%F,return,end

	err=setenv("INCLUDE",MSVCDir+'\INCLUDE;'+MSVCDir+'\MFC\INCLUDE;'+MSVCDir+'\ATL\INCLUDE;'+INCLUDE);
	if (err == %F) then bOK=%F,return,end
	
	LIB=getenv('LIB','ndef');
  if (LIB =='ndef') then  bOK=%F,return,end
  
  err=setenv("LIB",MSVCDir+'\LIB;'+MSVCDir+'\MFC\LIB;'+LIB);
  if (err == %F) then bOK=%F,return,end
  
  bOK=%T
else
  bOK=%F;
end
endfuntion
//-----------------------------------------------------------------------------