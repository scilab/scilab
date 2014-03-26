//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2012 - Allan CORNET
//
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at
// http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
    for i=1:size(modules,"*")
        files_nonregs = SCI + "/modules/" + modules(i) + "/tests/nonreg_tests/" + findfiles(SCI + "/modules/" + modules(i) + "/tests/nonreg_tests","*.*");
        files_unit = SCI + "/modules/" + modules(i) + "/tests/unit_tests" + findfiles(SCI + "/modules/" + modules(i) + "/tests/unit_tests","*.*");
        files_module = [files_nonregs;files_unit];
        files_nonregs = [];
        files_unit = [];
        for j = 1:size(files_module, "*")
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

    scilab_version = getversion("scilab");
    scilab_version_vstr = getversion();

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
        [SciFile, err] = FindAndReplace(SciFile, "#define ScilabBaseFilename", "#define ScilabBaseFilename """+ scilab_version_vstr + "_x64""");
    else
        [SciFile, err] = FindAndReplace(SciFile, "#define ScilabBaseFilename", "#define ScilabBaseFilename """+ scilab_version_vstr + """");
    end
    if err == %F then
        ret = err;
        return;
    end;

    [SciFile, err] = FindAndReplace(SciFile, "#define ScilabBaseDirectory", "#define ScilabBaseDirectory """+ scilab_version_vstr + """");
    if err == %F then
        ret = err;
        return;
    end;

    if win64() then
        [SciFile, err] = FindAndReplace(SciFile, "#define ScilabName", "#define ScilabName """ + scilab_version_vstr + " (64-bit)""");
    else
        [SciFile, err] = FindAndReplace(SciFile,"#define ScilabName","#define ScilabName """+ scilab_version_vstr +"""");
    end
    if err == %F then
        ret = err;
        return;
    end;

    if (scilab_version(3) == 0) then
        HTTP_MKL = "http://fileexchange.scilab.org/toolboxes/MKL/" + string(scilab_version(1)) + "." + string(scilab_version(2)) + "/files";
    else
        HTTP_MKL = "http://fileexchange.scilab.org/toolboxes/MKL/" + string(scilab_version(1)) + "." + string(scilab_version(2)) + "." + string(scilab_version(3)) + "/files";
    end

    [SciFile,err] = FindAndReplace(SciFile,"#define MKL_DOWNLOAD_HTTP ", "#define MKL_DOWNLOAD_HTTP """ + HTTP_MKL + """");
    if err == %F then
        ret = err;
        return;
    end;

    currentVersion = sprintf("%d.%d.%d",scilab_version(1),scilab_version(2),scilab_version(3));

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

    jre_used = system_getproperty("java.version");

    [SciFile,err] = FindAndReplace(SciFile,"#define javaUsed ","#define javaUsed """+jre_used+"""");
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

    if without_jre() then
        [SciFile,err] = FindAndReplace(SciFile,";#define SCILAB_WITHOUT_JRE","#define SCILAB_WITHOUT_JRE");
        if err == %F then
            ret = err;
            return;
        end;
    end

    [info_dyn, info_stat] = getdebuginfo();
    if grep(info_stat, "F2C") <> [] then
        [SciFile,err] = FindAndReplace(SciFile,";#define SCILAB_F2C","#define SCILAB_F2C");
        if err == %F then
            ret = err;
            return;
        end;
    end

    if (scilab_version(3) == 0) then
        ver_str = string(scilab_version(1)) + "." + string(scilab_version(2));
    else
        ver_str = string(scilab_version(1)) + "." + string(scilab_version(2)) + "." + string(scilab_version(3));
    end

    if win64() then
        arch_str = "win64";
    else
        arch_str = "win32";
    end

    MKL_BLASLAPACK_NAME = "blas-lapack-mkl-" + ver_str + "-" + arch_str + ".zip";
    MKL_COMMONS_NAME = "commons-mkl-" + ver_str + "-" + arch_str + ".zip";
    MKL_FFTW_NAME = "fftw-mkl-" + ver_str + "-" + arch_str + ".zip";

    [SciFile,err] = FindAndReplace(SciFile,"#define MKL_BLASLAPACK_PACKAGENAME","#define MKL_BLASLAPACK_PACKAGENAME ''" + MKL_BLASLAPACK_NAME + "''");
    if err == %F then
        ret = err;
        return;
    end;

    [SciFile,err] = FindAndReplace(SciFile,"#define MKL_COMMONS_PACKAGENAME","#define MKL_COMMONS_PACKAGENAME ''" + MKL_COMMONS_NAME + "''");
    if err == %F then
        ret = err;
        return;
    end;

    [SciFile,err] = FindAndReplace(SciFile,"#define MKL_FFTW_PACKAGENAME","#define MKL_FFTW_PACKAGENAME ''" + MKL_FFTW_NAME + "''");
    if err == %F then
        ret = err;
        return;
    end;

    if isdir(SCI + "/.atoms") <> %F then
        err = generateAdditionnalIss();
        if err == %F then
            ret = err;
            return;
        end;
    end


    mdelete(WSCI+"\"+ISSFilenameSource);
    printf("\nSaving %s\n",WSCI+"\"+ISSFilenameSource);
    ret = mputl(SciFile,WSCI+"\"+ISSFilenameSource);
endfunction
//------------------------------------------------------------------------------
function bOK = generateAdditionnalIss()
    bOK = %f;
    ATOMS_DIR = fullpath(SCI + "/contrib");
    dir_list = ls(ATOMS_DIR);
    dir_list = fullpath(ATOMS_DIR + filesep() + dir_list);
    dir_list = dir_list(isdir(dir_list) == %t);

    // remove default modules
    list_default_ATOMS = ["xcos_toolbox_skeleton", "toolbox_skeleton"];
    dir_list(grep(dir_list, list_default_ATOMS)) = [];
    if dir_list <> [] then
        name_list = basename(dir_list);
        ISS = "Source: contrib\" + name_list  +"\*.*; DestDir: {app}\contrib\" + name_list + "; Flags: recursesubdirs; Components: {#COMPN_SCILAB}";
        ISS = [ISS; "Source: .atoms\*.*; DestDir: {app}\.atoms; Flags: recursesubdirs; Components: {#COMPN_SCILAB}"];
    else
        ISS = "";
    end
    bOK = mputl(ISS, fullpath(SCI + "\contrib\external_modules.iss"));
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
