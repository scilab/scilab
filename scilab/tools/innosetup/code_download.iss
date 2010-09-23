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
