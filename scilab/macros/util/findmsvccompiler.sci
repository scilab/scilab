//-----------------------------------------------------------------------------
// Allan CORNET
// INRIA 2005
//-----------------------------------------------------------------------------
function MSCompiler=findmsvccompiler()
if MSDOS then
ierr=execstr("MSVS71=winqueryreg(''HKEY_LOCAL_MACHINE'',''SOFTWARE\Microsoft\VisualStudio\7.1\Setup\VC'',''ProductDir'');","errcatch");
if (ierr == 0) then
  MSCompiler='msvc71'; // Microsoft Visual Studio .NET 2003
  return;
else
  ierr=execstr("MSVS70=winqueryreg(''HKEY_LOCAL_MACHINE'',''SOFTWARE\Microsoft\VisualStudio\7.0\Setup\VC'',''ProductDir'');","errcatch");
  if (ierr == 0) then
    MSCompiler='msvc70';  // Microsoft Visual Studio .NET 2002
    return;
  else
    ierr=execstr("MSVS60=winqueryreg(''HKEY_LOCAL_MACHINE'',''SOFTWARE\Microsoft\DevStudio\6.0\Products\Microsoft Visual C++'',''ProductDir'');","errcatch");
    if (ierr == 0) then
      MSCompiler='msvc60';  // Microsoft Visual Studio 6
      return;
    else
      ierr=execstr("MSVS50=winqueryreg(''HKEY_LOCAL_MACHINE'',''SOFTWARE\Microsoft\DevStudio\5.0\Directories'',''ProductDir'');","errcatch");
      if (ierr == 0) then
        MSCompiler='msvc50';   // Microsoft Visual Studio 5
        return;
      else
        MSCompiler='unknown'; // unknown
        return;
      end
    end
  end
end
else
 MSCompiler='unknown'; // unknown
end
endfunction
//-----------------------------------------------------------------------------