//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
//
//-------------------------------------------------------------------------------
// Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
//
//------------------------------------------------------------------------------
// generates Scilab.iss for scilab 5.x in SCI directory
//------------------------------------------------------------------------------
function bOK = without_jre()
  bOK = %f;
  if ( isdef("ISS_WITHOUT_JRE") & isglobal(ISS_WITHOUT_JRE) ) then
    global ISS_WITHOUT_JRE;
    bOK = ISS_WITHOUT_JRE;
  else
    bOK = %f;
  end
endfunction
//------------------------------------------------------------------------------
function [StrFlux, retour] = FindAndReplace(StrFlux, StrFind, StrReplace)
  row = grep(StrFlux, StrFind);
  if (row == []) then
      printf("\n%s don""t find\n", StrFind);
      retour = %F;
  else
      DimRow = size(row);
      if DimRow(2) > 1 then // Take just first occurence
          StrFlux(row(1)) = StrReplace;
      else
          StrFlux(row) = StrReplace;
      end
      printf(".");
      retour=%T;
    end
endfunction
//------------------------------------------------------------------------------
function s = getTestsSize()
  modules = getmodules();
  total = 0;
  for i=1:size(modules,'*')
    files_nonregs = SCI + '/modules/' + modules(i) + '/tests/nonreg_tests/' + findfiles(SCI + '/modules/' + modules(i) + '/tests/nonreg_tests','*.*');
    files_unit = SCI + '/modules/' + modules(i) + '/tests/unit_tests' + findfiles(SCI + '/modules/' + modules(i) + '/tests/unit_tests','*.*');
    files_module = [files_nonregs;files_unit];
    files_nonregs = [];
    files_unit = [];
    for j = 1:size(files_module, '*')
      info = fileinfo(files_module(j));
      total = total + info(1);
    end
  end
  s = total / 10;
endfunction
//------------------------------------------------------------------------------
function ret = Update_Script_Innosetup(ISSFilenameSource)

    printf("Please Waiting ...\n");
    printf("Loading %s\n", ISSFilenameSource);
    SciFile = mgetl(ISSFilenameSource);
    printf("Creating %s ", WSCI + "\" + ISSFilenameSource);

    v = getversion("scilab");
    vstr = getversion();

    [SciFile, err] = FindAndReplace(SciFile, "#define TESTS_SIZE", "#define TESTS_SIZE " + sprintf("%20d",getTestsSize()));
    if err == %F then
        ret = err;
        return;
    end;


    [SciFile, err] = FindAndReplace(SciFile, "#define BinariesSourcePath", "#define BinariesSourcePath """ + WSCI + """");
    if err == %F then
        ret = err;
        return;
    end;

    if win64() then
    [SciFile, err] = FindAndReplace(SciFile, "#define ScilabBaseFilename", "#define ScilabBaseFilename """+ vstr + "_x64""");
  else
    [SciFile, err] = FindAndReplace(SciFile, "#define ScilabBaseFilename", "#define ScilabBaseFilename """+ vstr + """");
  end
    if err == %F then
        ret = err;
        return;
    end;

    [SciFile, err] = FindAndReplace(SciFile, "#define ScilabBaseDirectory", "#define ScilabBaseDirectory """+ vstr + """");
    if err == %F then
        ret = err;
        return;
    end;

    if win64() then
      [SciFile, err] = FindAndReplace(SciFile, "#define ScilabName", "#define ScilabName """ + vstr + " (64-bit)""");
    else
      [SciFile, err] = FindAndReplace(SciFile,"#define ScilabName","#define ScilabName """+ vstr +"""");
    end
  if err == %F then
     ret = err;
     return;
    end;

    w = strsplit(vstr, "-");
    HTTP_MKL = "http://www.scilab.org/download/master";
    HTTP_MKL_FFTW = "http://www.scilab.org/download/master";

  if isdef("w") then
    if size(w,"*") == 3 then
       if (w(3) == "master") then
         HTTP_MKL = "http://www.scilab.org/download/master";
         HTTP_MKL_FFTW = "http://www.scilab.org/download/master";
       else
         if or(w(2) == ["branch";"branches"]) then
           HTTP_MKL = "http://www.scilab.org/download/branches/" + w(3);
           HTTP_MKL_FFTW = "http://www.scilab.org/download/branches/" + w(3);
         end
       end
    end

    if size(w,"*") == 2 then
      HTTP_MKL = "http://www.scilab.org/download/" + w(2);
      HTTP_MKL_FFTW = "http://www.scilab.org/download/" + w(2);
    end
  end

    [SciFile,err] = FindAndReplace(SciFile,"#define MKL_DOWNLOAD_HTTP ", "#define MKL_DOWNLOAD_HTTP """ + HTTP_MKL + """");
    if err == %F then
      ret = err;
      return;
    end;

    [SciFile,err] = FindAndReplace(SciFile,"#define MKL_FFTW_DOWNLOAD_HTTP ", "#define MKL_FFTW_DOWNLOAD_HTTP """ + HTTP_MKL_FFTW + """");
    if err == %F then
      ret = err;
      return;
    end;

  currentVersion = sprintf("%d.%d.%d",v(1),v(2),v(3));

  [SciFile,err] = FindAndReplace(SciFile,"#define ScilabVersion ","#define ScilabVersion """+ currentVersion +"""");
  if err == %F then
    ret = err;
    return;
  end;

  CurrentDate = getdate();

  [SciFile,err] = FindAndReplace(SciFile,"#define CurrentYear ","#define CurrentYear """+string(CurrentDate(1))+"""");
  if err == %F then
     ret = err;
     return;
    end;

  jre_verstr = system_getproperty("java.specification.version");

  [SciFile,err] = FindAndReplace(SciFile,"#define javaSpecificationVersion ","#define javaSpecificationVersion """+jre_verstr+"""");
  if err == %F then
     ret = err;
     return;
    end;

  if with_module("scicos") then
    if with_modelica_compiler() then
      [SciFile,err] = FindAndReplace(SciFile,";#define WITH_MODELICAC","#define WITH_MODELICAC");
      if err == %F then
        ret = err;
        return;
      end;
    end
  end


  if win64() then
      [SciFile,err] = FindAndReplace(SciFile,";#define SCILAB_X64","#define SCILAB_X64");
    if err == %F then
        ret = err;
        return;
      end;
  end

  if ls(SCI + "/modules/helptools/chm/*.chm") <> [] then
      [SciFile,err] = FindAndReplace(SciFile,";#define WITH_CHM","#define WITH_CHM");
    if err == %F then
        ret = err;
        return;
      end;
  end

  if without_jre() then
      [SciFile,err] = FindAndReplace(SciFile,";#define SCILAB_WITHOUT_JRE","#define SCILAB_WITHOUT_JRE");
    if err == %F then
        ret = err;
        return;
      end;
  end

    mdelete(WSCI+"\"+ISSFilenameSource);
    printf("\nSaving %s\n",WSCI+"\"+ISSFilenameSource);
    mputl(SciFile,WSCI+"\"+ISSFilenameSource);
    ret = %T;

endfunction
//------------------------------------------------------------------------------
// Main
[units,typs,nams] = file();
path = fileparts(string(nams(1)),"path");
filename = fileparts(string(nams(1)),"fname");
extension = fileparts(string(nams(1)),"extension");

fileAndExt = filename+extension;

if or(fileAndExt == ["Create_ISS.sce","Create_ISS_nojre.sce"]) then
    SaveCurrentPath=pwd();
    if ~(SaveCurrentPath == path) then
        chdir(path);
    end

if ~(Update_Script_Innosetup("Scilab.iss") == %T ) then
        printf("\nScript aborted.\n");
    end
    chdir(SaveCurrentPath);

else
    printf("Error: name of this file isn""t ""Create_ISS.sce"" but %s\n",(filename+extension));
    printf("Please rename this file\n");
end
exit
//------------------------------------------------------------------------------
