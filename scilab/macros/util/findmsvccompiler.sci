//-----------------------------------------------------------------------------
// Allan CORNET
// INRIA 2005
//-----------------------------------------------------------------------------
function MSCompiler=findmsvccompiler()
  
  MSCompiler='unknown'; // unknown
  
  if MSDOS then
    ierr=-1;
    
    ierr=execstr("MSVS80Pro=winqueryreg(''HKEY_LOCAL_MACHINE'',''Software\Microsoft\VisualStudio\8.0\Setup\VS\Pro'',''ProductDir'');","errcatch");
    if (ierr == 0) then 
 	    MSCompiler='msvc80pro'; // Microsoft Visual 2005 Studio Professional
      return;
    end
    
    ierr=execstr("MSVS80std=winqueryreg(''HKEY_LOCAL_MACHINE'',''Software\Microsoft\VisualStudio\8.0\Setup\VS\Std'',''ProductDir'');","errcatch");
    if (ierr == 0) then 
 	    MSCompiler='msvc80std'; // Microsoft Visual 2005 Studio Standard
      return;
    end
    
   ierr=execstr("MSVS71=winqueryreg(''HKEY_LOCAL_MACHINE'',''SOFTWARE\Microsoft\VisualStudio\7.1\Setup\VC'',''ProductDir'');","errcatch");
   if (ierr == 0) then
     MSCompiler='msvc71'; // Microsoft Visual Studio .NET 2003
     return;
   end
   
   ierr=execstr("MSVS70=winqueryreg(''HKEY_LOCAL_MACHINE'',''SOFTWARE\Microsoft\VisualStudio\7.0\Setup\VC'',''ProductDir'');","errcatch");
   if (ierr == 0) then
     MSCompiler='msvc70';  // Microsoft Visual Studio .NET 2002
     return;
   end
 
   ierr=execstr("MSVS80EXPRESS=winqueryreg(''HKEY_LOCAL_MACHINE'',''Software\Microsoft\VCExpress\8.0\Setup\VS'',''ProductDir'');","errcatch");
     if (ierr == 0) then 
       ierr2=execstr("W2003SDK=winqueryreg(''HKEY_LOCAL_MACHINE'',''Software\Microsoft\MicrosoftSDK\InstalledSDKs\8F9E5EF3-A9A5-491B-A889-C58EFFECE8B3'',''Install Dir'');","errcatch");
       if (ierr2 == 0) then 
         MSCompiler='msvc80express'; // Microsoft Visual C++ Express 8.0
         return;
       else
         printf('\nWarning : Microsoft Visual C++ 2005 Express Edition has been detected,\nbut not Microsoft Platform SDK for Windows Server 2003 SP1.\nPlease install this SDK if you want to use dynamic link with scilab.\n');
       end  
   end
     
   ierr=execstr("MSVS60=winqueryreg(''HKEY_LOCAL_MACHINE'',''SOFTWARE\Microsoft\DevStudio\6.0\Products\Microsoft Visual C++'',''ProductDir'');","errcatch");
   if (ierr == 0) then
     MSCompiler='msvc60';  // Microsoft Visual Studio 6
     return;
   end
   
   ierr=execstr("MSVS50=winqueryreg(''HKEY_LOCAL_MACHINE'',''SOFTWARE\Microsoft\DevStudio\5.0\Directories'',''ProductDir'');","errcatch");
   if (ierr == 0) then
     MSCompiler='msvc50';   // Microsoft Visual Studio 5
     return;
   end
   
  else // MSDOS
    MSCompiler='unknown'; // unknown
  end
endfunction
//-----------------------------------------------------------------------------