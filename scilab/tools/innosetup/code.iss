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
//------------------------------------------------------------------------------
// Inno Setup Script (5.3 and more) for Scilab (UNICODE version required)
//
//------------------------------------------------------------------------------

function IsProcessorFeaturePresent(ProcessorFeature: DWORD): Boolean;
external 'IsProcessorFeaturePresent@kernel32.dll stdcall';

function isxdl_Download(hWnd: Integer; URL, Filename: PAnsiChar): Integer;
external 'isxdl_Download@files:isxdl.dll stdcall';

procedure isxdl_AddFile(URL, Filename: PAnsiChar);
external 'isxdl_AddFile@files:isxdl.dll stdcall';

procedure isxdl_AddFileSize(URL, Filename: PAnsiChar; Size: Cardinal);
external 'isxdl_AddFileSize@files:isxdl.dll stdcall';

function isxdl_DownloadFiles(hWnd: Integer): Integer;
external 'isxdl_DownloadFiles@files:isxdl.dll stdcall';

procedure isxdl_ClearFiles;
external 'isxdl_ClearFiles@files:isxdl.dll stdcall';

function isxdl_IsConnected: Integer;
external 'isxdl_IsConnected@files:isxdl.dll stdcall';

function isxdl_SetOption(Option, Value: PAnsiChar): Integer;
external 'isxdl_SetOption@files:isxdl.dll stdcall';

function isxdl_GetFileName(URL: PAnsiChar): PAnsiChar;
external 'isxdl_GetFileName@files:isxdl.dll stdcall';

function GetModuleHandle(lpModuleName: LongInt): LongInt;
external 'GetModuleHandleA@kernel32.dll stdcall';

var
        AboutModulesButton: TButton;

//------------------------------------------------------------------------------
function checkMD5Files(MD5File: String): Boolean;
    var
        TmpDirName: String;
        MD5sumFromTxt: String;
        MD5sumFromFile: String;
        filename: String;
        S: TArrayOfString;
        L,i: Longint;
        bOK: Boolean;
        MD5Compare: Integer;

    begin
        Result := true;
        TmpDirName := ExpandConstant('{tmp}') + '\';
        if FileExists(MD5File) then
            begin
                bOK := LoadStringsFromFile(MD5File, S);
                if (bOK = true) then
                    begin
                        L := GetArrayLength(S);
                        for i := 0 to L - 1 do
                            begin
                                MD5sumFromTxt := Copy(S[i], 0,Pos('|',S[i]) - 1 );
                                filename := Copy(S[i],Pos('|',S[i]) + 1, Length(S[i]) - Pos('|',S[i]));
                                if FileExists( TmpDirName + filename ) then
                                    begin
                                        MD5sumFromFile := GetMD5OfFile(TmpDirName + filename);
                                        MD5Compare := CompareText(MD5sumFromTxt, MD5sumFromFile);
                                        if (MD5Compare <> 0) then Result:= false;
                                    end
                                else
                                    begin
                                        Result:= false;
                                    end;
                            end;
                    end
                else begin
                    Result := false;
                end;
            end else begin
                Result := false;
            end;
    end;
//------------------------------------------------------------------------------
function checkMD5Files_MKL: Boolean;
    begin
        Result := checkMD5Files(ExpandConstant('{tmp}') + '\' + 'md5-mkl.txt');
    end;
//------------------------------------------------------------------------------
function checkMD5Files_MKL_FFTW: Boolean;
    begin
        Result := checkMD5Files(ExpandConstant('{tmp}') + '\' + 'md5-mkl-fftw.txt');
    end;
//------------------------------------------------------------------------------
function Install_MKL: Boolean;
    var
        TmpDirNameMKL: String;
        DestinationDir: String;

        bOK: Boolean;

    begin
        Result := false;

        TmpDirNameMKL := ExpandConstant('{tmp}') + '\';
        bOK := true;

        if not FileExists( TmpDirNameMKL + 'blasplus.dll' ) then bOK:= false;
        if not FileExists( TmpDirNameMKL + 'lapack.dll' ) then bOK:= false;
        if not FileExists( TmpDirNameMKL + 'readme-mkl.txt' ) then bOK:= false;

        if (bOK = true) then
            begin
                Result := true;
                DestinationDir := ExpandConstant('{app}') + '\bin\';

                bOK := FileCopy(TmpDirNameMKL + 'blasplus.dll', DestinationDir + 'blasplus.dll', false);
                if (bOK = false) then Result := false;

                bOK := FileCopy(TmpDirNameMKL + 'lapack.dll', DestinationDir + 'lapack.dll', false);
                if (bOK = false) then Result := false;

                bOK := FileCopy(TmpDirNameMKL + 'readme-mkl.txt', DestinationDir + 'readme-mkl.txt', false);
                if (bOK = false) then Result := false;
            end;
    end;
//------------------------------------------------------------------------------
function Install_MKL_FFTW: Boolean;
    var
        TmpDirNameMKL_FFTW: String;
        DestinationDir: String;

        bOK: Boolean;

    begin
        Result := false;

        TmpDirNameMKL_FFTW := ExpandConstant('{tmp}') + '\';
        bOK := true;

        if not FileExists( TmpDirNameMKL_FFTW + 'libfftw3-3.dll' ) then bOK:= false;
        if not FileExists( TmpDirNameMKL_FFTW + 'readme.txt' ) then bOK:= false;

        if (bOK = true) then
            begin
                Result := true;
                DestinationDir := ExpandConstant('{app}') + '\bin\fftw\';
                bOK := CreateDir(DestinationDir);
                if (bOK = false) then Result := false;

                bOK := FileCopy(TmpDirNameMKL_FFTW + 'libfftw3-3.dll', DestinationDir + 'libfftw3-3.dll', false);
                if (bOK = false) then Result := false;

                bOK := FileCopy(TmpDirNameMKL_FFTW + 'readme.txt', DestinationDir + 'readme-fftw.txt', false);
                if (bOK = false) then Result := false;
            end;
    end;
//------------------------------------------------------------------------------
function Download_MKL: Boolean;
    var
        hWnd: Integer;
        URL, DestinationTmpDirNameMKL: String;
        r : Integer;
    begin
        Result := False;

        isxdl_SetOption('label', CustomMessage('DownloadMKLLabel'));
        isxdl_SetOption('description', CustomMessage('DownloadMKLDescription'));
        isxdl_SetOption('resume', 'false');

        #ifdef SCILAB_X64
        URL := ExpandConstant('{#MKL_DOWNLOAD_HTTP}') +'/x64/';
        #else
        URL := ExpandConstant('{#MKL_DOWNLOAD_HTTP}')+ '/x86/';
        #endif

        DestinationTmpDirNameMKL := ExpandConstant('{tmp}') + '\';

        r := isxdl_IsConnected();
        if (r = 1) then
            begin
                isxdl_AddFile(URL + 'blasplus.dll', DestinationTmpDirNameMKL + 'blasplus.dll');
                isxdl_AddFile(URL + 'lapack.dll', DestinationTmpDirNameMKL + 'lapack.dll');
                isxdl_AddFile(URL + 'readme-mkl.txt', DestinationTmpDirNameMKL + 'readme-mkl.txt');
                isxdl_AddFile(URL + 'md5-mkl.txt', DestinationTmpDirNameMKL + 'md5-mkl.txt');

                hWnd := StrToInt(ExpandConstant('{wizardhwnd}'));
                if isxdl_DownloadFiles(hWnd) = 0 then
                    begin
                        Result := false;
                end else begin
                        Result := True;
                    end;
            end;
    end;
//------------------------------------------------------------------------------
function Download_MKL_FFTW: Boolean;
    var
        hWnd: Integer;
        URL, DestinationTmpDirNameMKL_FFTW: String;
        r : Integer;
    begin
        Result := False;

        isxdl_SetOption('label', CustomMessage('DownloadMKLFFTWLabel'));
        isxdl_SetOption('description', CustomMessage('DownloadMKLDescription'));
        isxdl_SetOption('resume', 'false');

        #ifdef SCILAB_X64
        URL := ExpandConstant('{#MKL_FFTW_DOWNLOAD_HTTP}') +'/x64/';
        #else
        URL := ExpandConstant('{#MKL_FFTW_DOWNLOAD_HTTP}')+ '/x86/';
        #endif

        DestinationTmpDirNameMKL_FFTW := ExpandConstant('{tmp}') + '\';

        r := isxdl_IsConnected();
        if (r = 1) then
            begin
                isxdl_AddFile(URL + 'libfftw3-3.dll', DestinationTmpDirNameMKL_FFTW + 'libfftw3-3.dll');
                isxdl_AddFile(URL + 'md5-mkl-fftw.txt', DestinationTmpDirNameMKL_FFTW + 'md5-mkl-fftw.txt');
                isxdl_AddFile(URL + 'readme.txt', DestinationTmpDirNameMKL_FFTW + 'readme.txt');

                hWnd := StrToInt(ExpandConstant('{wizardhwnd}'));
                if isxdl_DownloadFiles(hWnd) = 0 then
                    begin
                        Result := false;
                end else begin
                        Result := True;
                    end;
            end;
    end;
//------------------------------------------------------------------------------
function CreateModulesFile: Boolean;
    Var ModuleFileName : String;
    i : Integer;
    d : Integer;
    ModulesXmlFileLines: TArrayOfString;
    begin
        i := 0;

        setArrayLength(ModulesXmlFileLines, 256);
        for d := 0 to GetArrayLength(ModulesXmlFileLines)-1 do
          begin
            ModulesXmlFileLines[i] := '';
          end;

        ModuleFileName := ExpandConstant('{app}') +'\etc\modules.xml';

        ModulesXmlFileLines[i] := '<?xml version="1.0" encoding="UTF-8" standalone="no"?>'; i := i + 1;
        ModulesXmlFileLines[i] := '<!DOCTYPE modules SYSTEM "../modules/core/xml/modules.dtd">'; i := i + 1;
        ModulesXmlFileLines[i] := '<!-- =================== -->'; i := i + 1;
        ModulesXmlFileLines[i] := '<!--'; i := i + 1;
        ModulesXmlFileLines[i] := ' Modules list for Scilab'; i := i + 1;
        ModulesXmlFileLines[i] := ' @author Allan CORNET'; i := i + 1;
        ModulesXmlFileLines[i] := ' @date DIGITEO 2010'; i := i + 1;
        ModulesXmlFileLines[i] := ' ==================='; i := i + 1;
        ModulesXmlFileLines[i] := ' Do not touch if you do not know what you are doing'; i := i + 1;
        ModulesXmlFileLines[i] := ' Warning "core" module must be always the first module'; i := i + 1;
        ModulesXmlFileLines[i] := ' activate="yes" module will be launched ( or not "no")'; i := i + 1;
        ModulesXmlFileLines[i] := ' -->'; i := i + 1;
        ModulesXmlFileLines[i] := '<!-- =================== -->'; i := i + 1;
        ModulesXmlFileLines[i] := '<modules>'; i := i + 1;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="core" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="api_scilab" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="helptools" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="differential_equations" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="double" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="elementary_functions" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="functions" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="fileio" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_GRAPHICS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="graphics" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="renderer" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="graphic_export" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="boolean" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="cacsd" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="data_structures" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="shell" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="action_binding" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="console" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="completion" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="history_manager" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="gui" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="dynamic_link" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="integer" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="interpolation" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="io" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="output_stream" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="intersci" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="jvm" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="linear_algebra" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="localization" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="graph" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="ui_data" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="history_browser" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_OPTIM}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="optimization" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="overloading" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_POLY}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="polynomials" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_SIGPROC}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="signal_processing" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_SPARSE}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="sparse" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="special_functions" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="string" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_SYMB}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="symbolic" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="time" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="windows_tools" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_STATS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="statistics" activate="yes"/>'; i := i + 1;
            end;

        ModulesXmlFileLines[i] := '<!-- comments -->'; i := i + 1;

        if IsComponentSelected( ExpandConstant('{#COMPN_ARNOLDI}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="arnoldi" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_COMPATFUNCTIONS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="compatibility_functions" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_JAVASCI}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="javasci" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_MAP2SCI}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="maple2scilab" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_M2SCI}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="m2sci" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_MEXLIB}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="mexlib" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_TCLSCI}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="tclsci" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_TEXMACS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="texmacs" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_SOUND}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="sound" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_RANDLIB}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="randlib" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_DEVTOOLS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="development_tools" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="demo_tools" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_SPREADSHEET}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="spreadsheet" activate="yes"/>'; i := i + 1;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="call_scilab" activate="yes"/>'; i := i + 1;
            end;

        ModulesXmlFileLines[i] := '<!-- comments -->'; i := i + 1;

        if IsComponentSelected( ExpandConstant('{#COMPN_XCOS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="xcos" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="scicos" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="scicos_blocks" activate="yes"/>'; i := i + 1;
            end;

        ModulesXmlFileLines[i] := '<!-- comments -->'; i := i + 1;

        if IsComponentSelected( ExpandConstant('{#COMPN_PVM}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="pvm" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_FFTW}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="fftw" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_UMFPACK}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="umfpack" activate="yes"/>'; i := i + 1;
            end;

        ModulesXmlFileLines[i] := '<!-- comments -->'; i := i + 1;

        if (IsComponentSelected( ExpandConstant('{#COMPN_SCILAB}') )) then
            begin
                ModulesXmlFileLines[i] := '<module name="genetic_algorithms" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="simulated_annealing" activate="yes"/>'; i := i + 1;
                ModulesXmlFileLines[i] := '<module name="parameters" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_MATIO}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="matio" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_ATOMS}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="atoms" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_MODULES_MANAGER}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="modules_manager" activate="yes"/>'; i := i + 1;
            end;

        ModulesXmlFileLines[i] := '<module name="types" activate="yes"/>'; i := i + 1;
        ModulesXmlFileLines[i] := '<module name="hdf5" activate="yes"/>'; i := i + 1;

        if IsComponentSelected( ExpandConstant('{#COMPN_SCINOTES}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="scinotes" activate="yes"/>'; i := i + 1;
            end;

        if IsComponentSelected( ExpandConstant('{#COMPN_PARALLEL}') ) then
            begin
                ModulesXmlFileLines[i] := '<module name="parallel" activate="yes"/>'; i := i + 1;
            end;

        ModulesXmlFileLines[i] := '</modules>'; i := i + 1;

    Result := SaveStringsToFile(ModuleFileName, ModulesXmlFileLines, False);

    end;
//------------------------------------------------------------------------------
function DoTasksJustAfterInstall: Boolean;
    var
        bOK : Boolean;
    begin
        Result := true;

        bOK := CreateModulesFile();
        if (bOK = false) then
            begin
                Result := false;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_MKL_CPU_LIBRARY}') ) = true) then
            begin
                bOK := checkMD5Files_MKL();
                if (bOK = false) then
                    begin
                        SuppressibleMsgBox(CustomMessage('DoTasksJustAfterInstallMsg1') + #13 +
                        CustomMessage('DoTasksJustAfterInstallMsg2'),mbError, MB_OK, MB_OK );
                        Result := false;
                    end else begin
                        bOK := Install_MKL();
                        if (bOK = false) then
                            begin
                                Result := false;
                            end;
                    end;
            end;

        if (IsComponentSelected( ExpandConstant('{#COMPN_FFTW_MKL_LIBRARY}') ) = true) then
            begin

                bOK := checkMD5Files_MKL_FFTW();
                if (bOK = false) then
                    begin
                        SuppressibleMsgBox(CustomMessage('DoTasksJustAfterInstallMsg1') + #13 +
                        CustomMessage('DoTasksJustAfterInstallMsg3'),mbError, MB_OK, MB_OK );
                        Result := false;
                    end else begin

                        bOK := Install_MKL_FFTW();
                        if (bOK = false) then
                            begin
                                Result := false;
                            end;
                    end;
            end;

    end;
//------------------------------------------------------------------------------
function isWow64(): Boolean;
    begin
        result := false;
         if RegKeyExists(HKLM, 'SOFTWARE\Wow6432Node') then
            begin
                result := true;
            end;

    end;
//------------------------------------------------------------------------------
function GetJREVersion(): String;
    begin
        Result := '';
#ifdef SCILAB_X64
        RegQueryStringValue( HKLM, 'SOFTWARE\JavaSoft\Java Runtime Environment', 'CurrentVersion', Result );
#else

// 2 cas :
    if (isWow64() = true) then
        begin
            // Scilab 32 bits sur Windows 64 bits
            RegQueryStringValue( HKLM, 'SOFTWARE\Wow6432Node\JavaSoft\Java Runtime Environment', 'CurrentVersion', Result );
        end else
        begin
            // Scilab 32 bits sur Windows 32 bits
            RegQueryStringValue( HKLM, 'SOFTWARE\JavaSoft\Java Runtime Environment', 'CurrentVersion', Result );
        end;
#endif

    end;
//------------------------------------------------------------------------------
 function CheckJREVersion(): Boolean;
    var
        jreVersion:             String;
        minJREVersionRegistry:  String;
    begin
        //
        // Initialize min java version constant
        //
        minJREVersionRegistry := ExpandConstant('{#javaSpecificationVersion}');
        //
        //
        // now we check the version of the installed JRE
        jreVersion := GetJREVersion();
        //
        if ( jreVersion = '' ) then begin
            Result := false;
        end else if ( jreVersion < minJREVersionRegistry ) then begin
            Result := false;
        end else begin
            Result := true;
        end;
    end;
//------------------------------------------------------------------------------
 function VerifyJREVersion() : Boolean;
    var
        bJREVersion: Boolean;
    begin
        bJREVersion := CheckJREVersion();

        if ( bJREVersion <> true ) then begin
                SuppressibleMsgBox( CustomMessage('MsgBoxJavaDetection1') + #13 +
                    CustomMessage('MsgBoxJavaDetection2') + ExpandConstant('{#javaSpecificationVersion}') + '.',
                    mbError, MB_OK, MB_OK );
        end;

        Result := bJREVersion;
    end;
//------------------------------------------------------------------------------

procedure ButtonAboutModulesOnClick(Sender: TObject);
var
    ErrorCode: Integer;

begin
      if not ShellExec('', ExpandConstant('{#MODULES_LIST_WEB_PAGE}'),
                       '', '', SW_SHOW, ewNoWait, ErrorCode) then
        begin
            // handle failure if necessary
            SuppressibleMsgBox( CustomMessage('MsgBoxWebOpen'),mbError, MB_OK, MB_OK );
        end;
end;
//------------------------------------------------------------------------------
 function BackButtonClick(CurPageID: Integer): Boolean;
    begin
        Result := true;
        if (CurPageId = wpSelectProgramGroup) then
            begin
                AboutModulesButton.Visible := true;
            end else begin
                AboutModulesButton.Visible := false;
            end;
    end;
//------------------------------------------------------------------------------
 function NextButtonClick(CurPageID: Integer): Boolean;
    Var
        bRes : Boolean;
        ErrorCode: Integer;


    begin
        Result := true;

        if (CurPageID = wpWelcome) then
            begin
                #ifndef SCILAB_X64
                if (isWow64() = true) then
                    begin
                        SuppressibleMsgBox(CustomMessage('MsgBoxX64Ready'), mbInformation, MB_OK, MB_OK );
                    end;
                #endif

                if (IsProcessorFeaturePresent(10) = false) then
                    begin
                        bRes := false;
                        SuppressibleMsgBox(CustomMessage('MsgBoxSSERequired'), mbError, MB_OK, MB_OK );
                        Result := false;
                    end;
            end;

        if (CurPageId = wpSelectDir) then
            begin
                AboutModulesButton.Visible := true;
            end else begin
                AboutModulesButton.Visible := false;
            end;

        if (CurPageID =  wpReady) then
            begin
                if (IsComponentSelected( ExpandConstant('{#COMPN_MKL_CPU_LIBRARY}') ) = true) then
                    begin
                        bRes := Download_MKL();
                        if (bRes = false) then
                            begin
                                SuppressibleMsgBox(CustomMessage('NextButtonClickwpReadyMsg1'), mbError, MB_OK, MB_OK );
                            end;
                    end;

                if (IsComponentSelected( ExpandConstant('{#COMPN_FFTW_MKL_LIBRARY}') ) = true) then
                    begin
                        bRes := Download_MKL_FFTW();
                        if (bRes = false) then
                            begin
                                SuppressibleMsgBox(CustomMessage('NextButtonClickwpReadyMsg2'), mbError, MB_OK, MB_OK );
                            end;
                    end
            end;

        if (CurPageId = wpSelectComponents) then
            begin
//              if ( IsComponentSelected( ExpandConstant('{#COMPN_JRE}') ) = false ) then
//                  begin
//                      bRes := VerifyJREVersion();
//                      if ( bRes = false ) then
//                          begin
//                              Result := false;
//                          end;
//                  end;

                if ( (IsComponentSelected( ExpandConstant('{#COMPN_DEVTOOLS}') ) = false) and (IsComponentSelected( ExpandConstant('{#COMPN_TOOLBOX_SKELETON}') ) = true) ) then
                    begin
                            SuppressibleMsgBox( CustomMessage('MsgBoxDevToolsRequired1') + #13 +
                                CustomMessage('MsgBoxDevToolsRequired2'),
                                mbError, MB_OK, MB_OK );
                        Result := false;
                    end;

                    if ( (IsComponentSelected( ExpandConstant('{#COMPN_DEVTOOLS}') ) = false) and (IsComponentSelected( ExpandConstant('{#COMPN_TESTS}') ) = true) ) then
                    begin
                        SuppressibleMsgBox( CustomMessage('MsgBoxDevToolsRequired3') + #13 +
                                CustomMessage('MsgBoxDevToolsRequired2'),
                                mbError, MB_OK, MB_OK );
                        Result := false;
                    end;

            end;
    end;
//------------------------------------------------------------------------------
 function InitializeSetup: Boolean;
    Var
        Version: TWindowsVersion;
#ifdef SCILAB_WITHOUT_JRE
        bRes : Boolean;
#endif
    begin
        Result := True;
        GetWindowsVersionEx(Version);

        if Version.NTPlatform and (Version.Major > 4) then
            begin
                Result := True;
            end else begin
                    SuppressibleMsgBox(CustomMessage('MsgBoxWinVer'), mbCriticalError, MB_OK, MB_OK);
                  Result := False;
                  Exit;
            end
#ifdef SCILAB_WITHOUT_JRE
        bRes := CheckJREVersion();
        if ( bRes = false ) then
        begin
            SuppressibleMsgBox(CustomMessage('MsgBoxJRENotFound')+ '(' +ExpandConstant('{#javaSpecificationVersion}') + ')' + #13 +
                CustomMessage('MsgBoxJREURL')+ #13 +
                CustomMessage('MsgBoxJREReinstall')
                , mbCriticalError, MB_OK, MB_OK);
             Result := False;
             Exit;
        end else begin
            Result := True;
        end
#endif
    end;
//------------------------------------------------------------------------------
procedure CreateTheWizardPages;
var
  CancelButton: TButton;

begin

    CancelButton := WizardForm.CancelButton;

    AboutModulesButton := TButton.Create(WizardForm);
    AboutModulesButton.Left := WizardForm.ClientWidth - CancelButton.Left - CancelButton.Width;
    AboutModulesButton.Top := CancelButton.Top;
    AboutModulesButton.Width := CancelButton.Width * 2;

    AboutModulesButton.Caption := CustomMessage('ButtonAboutModules');

    AboutModulesButton.Height := CancelButton.Height;

    AboutModulesButton.OnClick := @ButtonAboutModulesOnClick;
    AboutModulesButton.Parent := CancelButton.Parent;
    AboutModulesButton.Visible := false;
end;
//------------------------------------------------------------------------------
procedure InitializeWizard();

begin
  CreateTheWizardPages;
end;
//------------------------------------------------------------------------------
