//
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) DIGITEO - 2010-2011 - Allan CORNET
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
function getInstallerDirectory: String;
  begin
    Result := ExtractFileDir(ExpandConstant('{srcexe}')); 
  end;
//------------------------------------------------------------------------------
function Download_Package(const packagefilename, URL, labeltxt, descriptiontxt: String): Boolean;
  var
    hWnd: Integer;
    DestinationTmpDirName: String;
    iConnect : Integer;
  begin
    Result := false;
    DestinationTmpDirName := ExpandConstant('{tmp}') + '\';
    if (FileExists(getInstallerDirectory() + '\' + packagefilename) = true) then
      begin
        Result := FileCopy(getInstallerDirectory() + '\' + packagefilename, DestinationTmpDirName + packagefilename, false);
      end
    else
      begin
        isxdl_SetOption('label', labeltxt);
        isxdl_SetOption('description', descriptiontxt);
        isxdl_SetOption('resume', 'false');
        iConnect := isxdl_IsConnected();
        if (iConnect = 1) then
          begin
            isxdl_AddFile(URL + packagefilename, DestinationTmpDirName + packagefilename);
            hWnd := StrToInt(ExpandConstant('{wizardhwnd}'));
            if isxdl_DownloadFiles(hWnd) = 0 then
              begin
                Result := false;
              end else begin
                FileCopy(DestinationTmpDirName + packagefilename, getInstallerDirectory() + '\' + packagefilename, false);
                Result := FileExists(DestinationTmpDirName + packagefilename);
              end;
          end;
      end;
  end;
//------------------------------------------------------------------------------
function Download_MKL: Boolean;
  var
    URL: String;
  begin
    URL := ExpandConstant('{#MKL_DOWNLOAD_HTTP}')+ '/';
    Result := Download_Package(ExpandConstant('{#MKL_BLASLAPACK_PACKAGENAME}'), URL, CustomMessage('DownloadMKLLabel'), CustomMessage('DownloadMKLDescription'));
  end;
//------------------------------------------------------------------------------
function Download_commons_MKL: Boolean;
  var
    URL: String;
  begin
    URL := ExpandConstant('{#MKL_DOWNLOAD_HTTP}')+ '/';
    Result := Download_Package(ExpandConstant('{#MKL_COMMONS_PACKAGENAME}'), URL, CustomMessage('DownloadMKLLabel'), CustomMessage('DownloadMKLDescription'));
  end;
//------------------------------------------------------------------------------
function Download_MKL_FFTW: Boolean;
  var
    URL: String;
  begin
    URL := ExpandConstant('{#MKL_DOWNLOAD_HTTP}')+ '/';
    Result := Download_Package(ExpandConstant('{#MKL_FFTW_PACKAGENAME}'), URL, CustomMessage('DownloadMKLFFTWLabel'), CustomMessage('DownloadMKLDescription'));  
  end;
//------------------------------------------------------------------------------
function Download_CHM: Boolean;
  var
    URL: String;
  begin
    URL := ExpandConstant('{#CHM_DOWNLOAD_HTTP}')+ '/';
    Result := Download_Package(ExpandConstant('{#CHM_PACKAGENAME}'), URL, CustomMessage('DownloadCHMLabel'), CustomMessage('DownloadCHMDescription'));  
  end;
//------------------------------------------------------------------------------
function Install_Package(const packagefullfilename, pathdest: String):  Boolean;
  var
    TmpDirName: String;
    listfilesArray: TArrayOfString;
    listfiles_name: String;
    i : integer;

  begin
    Result := True;
    TmpDirName := ExpandConstant('{tmp}') + '\';
    listfiles_name := TmpDirName + 'list_files.txt';
    
    if (Unzip(packagefullfilename) = true) then
      begin
        if (LoadStringsFromFile(listfiles_name, listfilesArray) = true) then
          begin
            for i := 0 to GetArrayLength(listfilesArray) - 1 do
              begin
                if (FileCopy(TmpDirName + listfilesArray[i], pathdest + listfilesArray[i], false) = false) then
                  begin
                    Result := False;
                  end;
                DeleteFile(TmpDirName + listfilesArray[i]);
              end;
          end
        else
          begin
            Result := False;
          end;
      end
    else
      begin
        Result := False;
      end; 
  end;
//------------------------------------------------------------------------------
function Install_MKL: Boolean;
  var
    TmpDirName: String;
    fullnamePackage: String;
    destinationDirectory: String;

  begin
    TmpDirName := ExpandConstant('{tmp}') + '\';
    fullnamePackage := TmpDirName + ExpandConstant('{#MKL_BLASLAPACK_PACKAGENAME}');
    destinationDirectory := ExpandConstant('{app}') + '\bin\';
    
    if FileExists(fullnamePackage) then
      begin
        Result := Install_Package(fullnamePackage, destinationDirectory);
      end
    else
      begin
        Result := False;
      end;

  end;
//------------------------------------------------------------------------------
function Install_commons_MKL: Boolean;
  var
    TmpDirName: String;
    fullnamePackage: String;
    destinationDirectory: String;

  begin
    TmpDirName := ExpandConstant('{tmp}') + '\';  
    fullnamePackage := TmpDirName + ExpandConstant('{#MKL_COMMONS_PACKAGENAME}');
    destinationDirectory := ExpandConstant('{app}') + '\bin\';
    
    if FileExists(fullnamePackage) then
      begin
        Result := Install_Package(fullnamePackage, destinationDirectory);
      end
    else
      begin
        Result := False;
      end;

  end;
//------------------------------------------------------------------------------
function Install_MKL_FFTW: Boolean;
  var
    TmpDirName: String;
    fullnamePackage: String;
    destinationDirectory: String;

  begin
    TmpDirName := ExpandConstant('{tmp}') + '\';
    fullnamePackage := TmpDirName + ExpandConstant('{#MKL_FFTW_PACKAGENAME}');
    destinationDirectory := ExpandConstant('{app}') + '\bin\fftw\';
    
    if FileExists(fullnamePackage) then
      begin
        Result := Install_Package(fullnamePackage, destinationDirectory);
      end
    else
      begin
        Result := False;
      end;

  end;
//------------------------------------------------------------------------------
function Install_CHM: Boolean;
  var
    TmpDirName: String;
    fullnamePackage: String;
    destinationDirectory: String;

  begin
    TmpDirName := ExpandConstant('{tmp}') + '\';
    fullnamePackage := TmpDirName + ExpandConstant('{#CHM_PACKAGENAME}');
    destinationDirectory := ExpandConstant('{app}') + '\modules\helptools\chm\';
    ForceDirectories(destinationDirectory);
    if FileExists(fullnamePackage) then
      begin
        Result := Install_Package(fullnamePackage, destinationDirectory);
      end
    else
      begin
        Result := False;
      end;

  end;
//------------------------------------------------------------------------------
